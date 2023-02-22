/** @file bb_tx_proc.h
*
* @brief Blackboard Tx Processor definition
*
* @par
* Copyright (c) 2018 Sunrise Labs, Inc.
* All rights reserved.
*/

#ifndef BB_TX_PROC_H
#define BB_TX_PROC_H

#include "ican_tx.h"
#include "can_spec.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


typedef struct
{
    uint32_t maxDelay;
    ICANTx_T* transmitters;
    uint32_t numTransmitters;

} BlackboardTxProcessor_T;

void bb_tx_proc_init(BlackboardTxProcessor_T * bbTxProc,
                     uint32_t maxDelay,
                     ICANTx_T* transmitters,
                     uint32_t numTransmitters,
                     uint32_t currTimestampTicks);

uint32_t bb_tx_proc_process_tx(const BlackboardTxProcessor_T * const bbTxProc,
                               const uint32_t currTimestampTicks,
                               const uint32_t prevTimestampTicks);

uint32_t bb_tx_proc_tx_message(uint32_t index,
                   uint32_t elapsedTicks,
                   const CANElement_T * canElement,
                   const BlackboardTxProcessor_T * const bbTxProc,
                   uint32_t canChannel,
                   uint32_t maskTxChannel,
                   uint32_t minimumOffsetTicks);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // BB_TX_PROC_H
