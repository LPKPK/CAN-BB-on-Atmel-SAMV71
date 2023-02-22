/**
 * Copyright (C) 2018 Sunrise Labs, Inc.
 * All rights reserved
 *
 * @brief Tool to determine schedulability of CAN configuration
 *
 * This C version is a translation of CAN_is_schedulable.lua, which works on .csv files.
 * This C version works directly from the header file used by the application code.
 *
 *  Based on:
 *  Controller Area Network (CAN) schedulability analysis: Refuted, revisited and revised
 *  Robert I. Davis Alan Burns & Reinder J. Bril & Johan J. Lukkien
 *  Published online: 30 January 2007
 *
 * @par
 * @copyright Copyright (C) 2017-2018 Sunrise Labs, Inc. All rights reserved.
 *
 * @file
 **/

#ifndef CAN_IS_SCHEDULABLE_H
#define CAN_IS_SCHEDULABLE_H

#include "can_spec_sched.h" // This is the schedule to check
#include <stdint.h>

#ifdef CAN_CMD_LINE_BUILD //used by the run_CAN_is_schedulable script to output to csv
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#endif

// -- config
// Tbit   = 1 microsecond by default
// IDsize = 11 by default; can be 29

static const uint32_t Tbit = 1U;
static const uint32_t IDsize = 11U;


//lint -e{958} padding needed to align end of structure, not necessary on this compile-time logic
typedef struct
{
    // -- inputs (all subscripted by message number "m", assigned using priority sort of messages)
    const char_T *Ns; // the message name
    uint32_t Pm; // the priority of message m i.e., its CAN message ID (the paper uses m as Pm)
    uint32_t Sm; // number of data bytes in message m
    uint32_t Tm; // period of message m in microseconds
    uint32_t Dm; // deadline of message m in microseconds
    uint32_t Jm; // queuing jitter in microseconds

    // -- calculated
    uint32_t Cm; // max transmission time of message m in microseconds
    uint32_t Wm; // queuing delay of message m in microseconds = Bm + Im
    uint32_t Bm; // blocking time (due to lower priority messages in progress) of message m in microseconds
    uint32_t Im; // interference time (due to higher priority messages) of message m in microseconds
    uint32_t Rm; // worst case response time of message m in microseconds = Jm + Wm + Cm in simple case
    uint32_t Ym; // length in microseconds of the priority level-m busy period (called tm in paper)
    uint32_t Qm; // number of instances of message m that may become ready before end of busy period Ym
    float64_T Um; // bus utilization for messages of priority Pm and higher; must be < 1 to be schedulable
    uint32_t Nm; // number of instances of message m that need to be buffered due to scheduling
    bool_T     Ng; // true if the message is not schedulable

} msg_v;


//used to sort the messages by priority of message (id)
static constexpr void bubble_sort(msg_v * const msgs, const uint32_t numMsgs)
{
    for (uint32_t i = 0;  i < (numMsgs - 1);  i++)
    {
        for (uint32_t j = 0;  j < ((numMsgs - i) - 1);  j++)
        {
            if (msgs[j].Pm > msgs[j+1].Pm) // sort so lower numbered Pm come first in result
            {
                const msg_v temp  = msgs[j];
                msgs[j] = msgs[j+1];
                msgs[j+1]= temp;
            }
        }
    }
}

constexpr void init_msgs (const uint32_t jitter_usec, const uint32_t deadline_as_percent_of_period, msg_v * const msgs)
{
    for (uint32_t i = 0U; i < static_cast<uint32_t>(CAN_ELEM_LAST_MSG); i++)
    {
        msgs[i].Ns = can_spec_can_element_sched_list[i].canMsgInfo.name;
        msgs[i].Pm = can_spec_can_element_sched_list[i].canMsgInfo.id;
        msgs[i].Sm = can_spec_can_element_sched_list[i].canMsgInfo.numBytes;
        msgs[i].Tm = can_spec_can_element_sched_list[i].canMsgInfo.period_ms * 1000U;
        msgs[i].Jm = jitter_usec;
#if CIS_VERBOSE
        // Nice feedback for human, inconvenient for scripting use of this utility
        fprintf(stderr, "i = %d\n", i);
        fprintf(stderr, "msg name = %s\n", msgs[i].Ns);
        fprintf(stderr, "msg Tm = %d\n", msgs[i].Tm);
#endif
        // It would be ideal if can_msg_table defined deadline, but right now it doesn't.
        // Various default deadlines could be justified:
        // 1. The message period, to keep queue sizes below 1
        // 2. The message period * XX% (XX < 100) to provide some slop
        // 3. The message period minus the jitter, to keep queue sizes below 1, with some slop
        // Since jitter is always positive (we are using a timer interrupt to trigger queueing),
        // and the algorithm takes jitter into account in verifying that deadlines are met,
        // option 3 is overkill.
#if 0
        msgs[i].Dm = msgs[i].Tm - jitter_usec;
#else
        msgs[i].Dm = (msgs[i].Tm * deadline_as_percent_of_period) / 100U;
#endif
    }

    //now sort the msgs so lower number Pm are first in result
    bubble_sort(msgs, static_cast<uint32_t>(CAN_ELEM_LAST_MSG));
}


// -- calculate Cm
//
// for i = 1, Nmessages do
//     messages[i].Cm = (((IDsize == 11) and 55 or 80) + 10 * messages[i].Sm) * Tbit
// end
//
static constexpr void calculate_Cm (msg_v * const msgs)
{
    for (uint32_t i = 0U; i < static_cast<uint32_t>(CAN_ELEM_LAST_MSG); i++)
    {
        msgs[i].Cm = (((IDsize == 11U) ? 55U : 80U) + (10U * msgs[i].Sm)) * Tbit;
    }
}

// -- calculate Bm (max Ck for k lower priority than m)
//
// for i = Nmessages, 1, -1 do
//     messages[i].Bm = 0 -- initial value
//     for j = Nmessages, i + 1, -1 do -- messages of lower priority
//         if messages[i].Bm < messages[j].Cm then messages[i].Bm = messages[j].Cm end
//     end
// end
//
static constexpr void calculate_Bm (msg_v * const msgs)
{
    // For-loop stops because of intentional wrap to max-u32.  Lint notices this and warns accordingly, hence the suppression
    for (uint32_t i = static_cast<uint32_t>(CAN_ELEM_LAST_MSG) - 1U; //lint !e442 for statement condition and increment directions are inconsistent
         i < static_cast<uint32_t>(CAN_ELEM_LAST_MSG);
         i--)
    {
        msgs[i].Bm = 0U; // initial value

        // for messages of lower priority...
        for (uint32_t j = static_cast<uint32_t>(CAN_ELEM_LAST_MSG) - 1U; j > i; j--)
        {
            if (msgs[i].Bm < msgs[j].Cm)
            {
                msgs[i].Bm = msgs[j].Cm;
            }
        }
    }
}

// -- calculate Wm (requires iteration of a recurrence relation)
//
// for i = 1, Nmessages do
//
//     local Wm_last = -1
//
//     messages[i].Wm = messages[i].Bm -- initial value
//
//     while Wm_last ~= messages[i].Wm do
//
//         Wm_last = messages[i].Wm
//
//         messages[i].Im = 0
//
//         for k = 1, i - 1 do
//             messages[i].Im = messages[i].Im
//              + messages[k].Cm * math.ceil((Wm_last + messages[k].Jm + Tbit) / messages[k].Tm)
//         end
//
//         messages[i].Wm = messages[i].Bm + messages[i].Im
//
//         if (messages[i].Jm + messages[i].Wm + messages[i].Cm) > messages[i].Dm then
//             break -- message is not schedulable
//         end
//     end
// end
//
static const void calculate_Wm (msg_v * const msgs)
{
#if CIS_VERBOSE
    // Nice feedback for human, inconvenient for scripting use of this utility
    fprintf(stderr, "In Wm\n");
#endif
    for (uint32_t i = 0U; i < static_cast<uint32_t>(CAN_ELEM_LAST_MSG); i++)
    {
        fprintf(stderr, "i = %d\n", i);
        uint32_t Wm_last = UINT32_MAX;

        msgs[i].Wm = msgs[i].Bm; // initial value

        while (Wm_last != msgs[i].Wm)
        {
            Wm_last = msgs[i].Wm;
            msgs[i].Im = 0U;

            for (uint32_t k = 0U; k < i; k++)
            {
#if CIS_VERBOSE
                // Nice feedback for human, inconvenient for scripting use of this utility
                fprintf(stderr, "k = %d\n", k);
                fprintf(stderr, "msg name = %s\n", msgs[k].Ns);
                fprintf(stderr, "msg Tm = %d\n", msgs[k].Tm);
#endif
                const uint32_t ceil = (((Wm_last + msgs[k].Jm + Tbit + msgs[k].Tm) - 1U) / msgs[k].Tm); // ceiling
                msgs[i].Im += msgs[k].Cm * ceil;
            }

            msgs[i].Wm = msgs[i].Bm + msgs[i].Im;

            if ((msgs[i].Jm + msgs[i].Wm + msgs[i].Cm) > msgs[i].Dm)
            {
                break; // message is not schedulable
            }
        }
    }
}

// -- calculate Ym (requires iteration of a recurrence relation) and Um and Qm
//
// for i = 1, Nmessages do
//
//     local um = 0
//
//     for k = 1, i do um = um + (messages[k].Cm / messages[k].Tm) end
//
//     messages[i].Um = um
//
//     if (um >= 1) then
//
//         -- the calculation below will not converge
//         messages[i].Ym = 1.0/0.0
//
//     else
//
//         local Ym_last = -1
//
//         messages[i].Ym = messages[i].Cm -- initial value
//
//         while Ym_last ~= messages[i].Ym do
//
//             Ym_last = messages[i].Ym
//
//             local im = 0
//
//             for k = 1, i do
//                 im = im + messages[k].Cm * math.ceil((Ym_last + messages[k].Jm) / messages[k].Tm)
//             end
//
//             messages[i].Ym = messages[i].Bm + im
//         end
//     end
//
//     messages[i].Qm = math.ceil((messages[i].Ym + messages[i].Jm) / messages[i].Tm)
// end
//
static constexpr void calculate_Ym (msg_v * const msgs)
{
    for (uint32_t i = 0U; i < static_cast<uint32_t>(CAN_ELEM_LAST_MSG); i++)
    {
        float64_T um = 0.0;

        for (uint32_t k = 0U; k <= i; k++)
        {
            um += static_cast<float64_T>(msgs[k].Cm) / static_cast<float64_T>(msgs[k].Tm);
        }

        msgs[i].Um = um;

        if (um >= 1.0)
        {
            // the calculation below will not converge
            msgs[i].Ym = UINT32_MAX; // NaN !?
        }
        else
        {
            uint32_t Ym_last = UINT32_MAX;

            msgs[i].Ym = msgs[i].Cm; // initial value

            while (Ym_last != msgs[i].Ym)
            {
                uint32_t im = 0U;
                Ym_last = msgs[i].Ym;

                for (uint32_t k = 0U; k <= i; k++)
                {
                    const uint32_t ceil = (((Ym_last + msgs[k].Jm + msgs[k].Tm) - 1U) / msgs[k].Tm); // ceiling
                    im += msgs[k].Cm * ceil;
                }

                msgs[i].Ym = msgs[i].Bm + im;
            }
        }

        msgs[i].Qm = ((msgs[i].Ym + msgs[i].Jm + msgs[i].Tm) - 1U) / msgs[i].Tm; // ceiling
    }
}

// -- calculate Rm
// -- if Qm <= 1 then Wm is correct, otherwise we have to check for multiple queued instances
//
// for i = 1, Nmessages do
//
//     if messages[i].Um >= 1 then
//         -- the calculation below will not converge
//         messages[i].Rm = 1.0/0.0
//
//     elseif messages[i].Qm <= 1 then
//         -- all set, Wm is OK
//         messages[i].Rm = messages[i].Jm + messages[i].Wm + messages[i].Cm
//
//     else
//         messages[i].Rm = -1 -- will be replaced
//
//         for q = 0, messages[i].Qm - 1 do
//
//             local rmq
//             local Wm_last = -1
//
//             messages[i].Wm = messages[i].Bm + q * messages[i].Cm -- initial value
//
//             while Wm_last ~= messages[i].Wm do
//
//                 Wm_last = messages[i].Wm
//
//                 messages[i].Im = 0
//
//                 for k = 1, i - 1 do
//                     messages[i].Im = messages[i].Im
//                      + messages[k].Cm * math.ceil((Wm_last + messages[k].Jm + Tbit) / messages[k].Tm)
//                 end
//
//                 messages[i].Wm = messages[i].Bm + q * messages[i].Cm + messages[i].Im
//
//                 rmq = messages[i].Jm + messages[i].Wm + messages[i].Cm - q * messages[i].Tm
//
//                 if rmq > messages[i].Dm then
//                     messages[i].Rm = rmq
//                     break -- message is not schedulable
//                 end
//             end
//
//             if (rmq > messages[i].Rm) then messages[i].Rm = rmq end
//
//         end
//     end
//
//     if messages[i].Rm > messages[i].Dm then
//         messages[i].Ng = "UNSCHEDULABLE!"
//         n_unschedulable = n_unschedulable + 1
//     end
//
//     messages[i].Nm = math.ceil(messages[i].Rm / messages[i].Tm)
// end
//
static constexpr uint32_t calculate_Rm (msg_v * const msgs)
{
    uint32_t unscheduleable_count = 0U;

    for (uint32_t i = 0U; i < static_cast<uint32_t>(CAN_ELEM_LAST_MSG); i++)
    {
        if (msgs[i].Um >= 1.0)
        {
            // the calculation below will not converge
            msgs[i].Rm = UINT32_MAX; // NaN !?
        }
        else if (msgs[i].Qm <= 1U)
        {
            // all set, Wm is OK
            msgs[i].Rm = msgs[i].Jm + msgs[i].Wm + msgs[i].Cm;
        }
        else
        {
            msgs[i].Rm = UINT32_MAX; // will be replaced

            for (uint32_t q = 0U; q < msgs[i].Qm; q++)
            {
                uint32_t rmq = 0U;
                uint32_t Wm_last = UINT32_MAX;

                msgs[i].Wm = msgs[i].Bm + (q * msgs[i].Cm); // initial value

                while (Wm_last != msgs[i].Wm)
                {
                    Wm_last = msgs[i].Wm;
                    msgs[i].Im = 0U;

                    for (uint32_t k = 0U; k < i; k++)
                    {
                        const uint32_t ceil = ((Wm_last + msgs[k].Jm + Tbit + msgs[k].Tm) - 1U) / msgs[k].Tm;
                        msgs[i].Im += msgs[k].Cm * ceil;
                    }

                    msgs[i].Wm = msgs[i].Bm + (q * msgs[i].Cm) + msgs[i].Im;

                    rmq = (msgs[i].Jm + msgs[i].Wm + msgs[i].Cm) - (q * msgs[i].Tm);

                    if (rmq > msgs[i].Dm)
                    {
                        msgs[i].Rm = rmq;
                        break; // message is not schedulable
                    }
                }

                if (rmq > msgs[i].Rm)
                {
                    msgs[i].Rm = rmq;
                }
            }
        }

        if (msgs[i].Rm > msgs[i].Dm)
        {
            msgs[i].Ng = true;
            unscheduleable_count += 1U;
        }

        msgs[i].Nm = ((msgs[i].Rm + msgs[i].Tm) - 1U) / msgs[i].Tm; // ceiling
    }
    return unscheduleable_count;
}


#endif // CAN_IS_SCHEDULABLE_H
