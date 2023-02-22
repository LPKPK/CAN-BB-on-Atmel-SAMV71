/** @file bb_tx_proc.c
 *
 * @brief Common Blackboard Tx Processor module
 *
 * Processes transmission of Blackboard elements through the CAN bus using supplied 
 *
 * @par
 * Copyright (c) 2021 Sunrise Labs, Inc.
 * All rights reserved.
 */
#include "can_elem.h"
#include "bb_tx_proc.h"
#include "blackboard.h"
#include <asf.h>

/**
 * @brief Initialization function for a blackboard tx processor struct.
 * Sets the wakeup offsets for each element based on the current time.
 * 
 * @param bbTxProc Pointer to the blackboard tx processor struct
 * @param maxDelay Integer to store in the blackboard tx processor
 * @param transmitters Pointer to an array of transmitter structs
 * @param numTransmitters Integer representing the length of the previous array
 * @param currTimestampTicks Integer representing the current time in ticks
 */
void bb_tx_proc_init(BlackboardTxProcessor_T * bbTxProc,
                     uint32_t maxDelay,
                     ICANTx_T* transmitters,
                     uint32_t numTransmitters,
                     uint32_t currTimestampTicks)
{
    bbTxProc->maxDelay = maxDelay;
    bbTxProc->transmitters = transmitters;
    bbTxProc->numTransmitters = numTransmitters;

    for (uint32_t i = 0U; i < (uint32_t)(CAN_ELEM_LAST_MSG); ++i)
    {
        BlackboardElement_T * bbElement = bb_get_element((CANElementIndex_T)i);
        bb_elem_set_wakeup_offset(bbElement, currTimestampTicks + i);
    }
}

/**
 * @brief Processes the transmission of CAN messages by cycling through the Blackboard elements

 * @param bbTxProc Pointer to the blackboard tx processor struct
 * @param currTimestampTicks The current time
 * @param prevTimestampTicks The previous time that this function was called
 *
 * @return The minimum offset ticks - the ticks to wait until before waking up again
 */
uint32_t bb_tx_proc_process_tx(const BlackboardTxProcessor_T * const bbTxProc,
                               const uint32_t currTimestampTicks,
                               const uint32_t prevTimestampTicks)
{
    const uint32_t elapsedTicks = currTimestampTicks - prevTimestampTicks;

    uint32_t minimumOffsetTicks = bbTxProc->maxDelay;

    // For each Can Element, Check to see if it is transmitted on one of the CAN channels If so,
    // check to see if it is time to transmit the element.  If it is, get the element data and
    // transmit it.
    for (uint32_t i = 0U; i < (uint32_t)(CAN_ELEM_LAST_MSG); ++i)
    {
        const CANElement_T * canElement = can_elem_get_can_elem_idx((CANElementIndex_T)i);
        //printf("msgID: %d, txChan:%d \r\n", canElement->canMsgInfo.id, canElement->txChan);
        if ((uint32_t)canElement->txChan > 0UL)
        {
            for(uint32_t canChannel = 0U; canChannel < bbTxProc->numTransmitters; canChannel++)
            {
                const uint32_t maskTxChannel = (uint32_t)1U << canChannel;
                if (((uint32_t)canElement->txChan & maskTxChannel) > 0UL)
                {
                    minimumOffsetTicks = bb_tx_proc_tx_message(i, elapsedTicks, canElement, bbTxProc, canChannel, maskTxChannel, minimumOffsetTicks);
                }
            }
        }
    }
    return minimumOffsetTicks;
}


/**
 * @brief Processes the transmission of a single CAN message
 *
 * @param index The index of the CANElement
 * @param elapsedTicks Number of ticks since this CAN message was last sent
 * @param canElement Pointer to the CAN element to send
 * @param bbTxProc Pointer to the blackboard tx processor struct
 * @param canChannel The channel on which to transmit
 * @param maskTxChannel A bitmask of the channel, sent into the callback
 * @param minimumOffsetTicks Ticks to wait until before waking up again
 */
uint32_t bb_tx_proc_tx_message(uint32_t index,
                   uint32_t elapsedTicks,
                   const CANElement_T * canElement,
                   const BlackboardTxProcessor_T * const bbTxProc,
                   uint32_t canChannel,
                   uint32_t maskTxChannel,
                   uint32_t minimumOffsetTicks)
{
    BlackboardElement_T * bbElement = bb_get_element((CANElementIndex_T)index);
    if (!bb_elem_decrement_wakeup_offset(bbElement, elapsedTicks))
    {
        const uint32_t overflow_ticks = bb_elem_compute_overflow_ticks(bbElement, elapsedTicks);
        const CANMsgInfo_T * msgInfo = &canElement->canMsgInfo;

        // The new offset for this element is never more than 1x its period.
        // If messages are missed, it does not send more than one.
        // Note - BB_PORT_MS_TO_TICKS must be defined in can_spec.h
        const uint32_t period_ticks = BB_PORT_MS_TO_TICKS(msgInfo->period_ms);
        bb_elem_set_wakeup_offset(bbElement, period_ticks - (overflow_ticks % period_ticks));

        uint32_t tx_data[2] = {0, 0};
        bb_elem_get_data_for_can(bbElement, &tx_data[0], &tx_data[1]);
        
        const ICANTx_T * const transmitter = &bbTxProc->transmitters[canChannel];
        transmitter->transmitFn(transmitter->context, msgInfo->id, (uint8_t *)(&tx_data[0]), (uint8_t)msgInfo->numBytes);

        if (canElement->callback.callbackFn != NULL)
        {
            canElement->callback.callbackFn(canElement->callback.callbackCtx,
                                            (CanChannel_T)maskTxChannel, // Tx Channel (masked)
                                            CAN_NONE);                 // Rx Channel is N/A
        }
        // else Callback container is null, don't call
    }
    return bb_elem_compute_minimum_offset_ticks(bbElement, minimumOffsetTicks);
}
