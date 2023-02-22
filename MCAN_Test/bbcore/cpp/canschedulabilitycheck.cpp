/** @file canschedulabilitycheck.cpp
*
* @brief Compile time checking of CAN schedulability.
*
* @par
* Copyright (c) 2018 Sunrise Labs, Inc.
* All rights reserved.
*/

#include "CAN_is_schedulable.h" // This is the tool for checking schedulability

//determines if the CAN messages are schedulable based on jitter and deadline as a percent of period
static const bool_T isCanSchedulable (const uint32_t jitter_usec, const uint32_t deadline_as_percent_of_period)
{
    msg_v msgs[CAN_ELEM_LAST_MSG] = {};

    //const int init_result = init_msgs(jitter_usec, deadline_as_percent_of_period, msgs);
    static_assert(init_msgs(500, 90, msgs) == 99, "Doesn't");

    calculate_Cm(msgs);
    calculate_Bm(msgs);
    calculate_Wm(msgs);
    calculate_Ym(msgs);

    const uint32_t unscheduleable_count = calculate_Rm(msgs);

    if (unscheduleable_count > 0U)
    {
        //unschedulable
        return false;
    }
    else
    {
        //schedulable
        return true;
    }
}

// default queuing jitter of 500 microseconds
//static_assert(isCanSchedulable(500, 90),  "CAN is not schedulable. Run CAN schedulability tool to check CSV outputs.");
