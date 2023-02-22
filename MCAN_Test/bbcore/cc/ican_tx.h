/** @file ican_tx.h
*
* @brief Defines an interface for transmitting CAN messages that is platform agnostic.
*
* @par
* Copyright (c) 2021 Sunrise Labs, Inc.
* All rights reserved.
*/

#ifndef ICAN_TX_H
#define ICAN_TX_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef void * ICANTxCtx_T;

typedef void (* ICANTxFn_T)(ICANTxCtx_T , uint32_t msgID, uint8_t * data, uint8_t length);

typedef struct
{
    ICANTxFn_T transmitFn;
    ICANTxCtx_T context;

} ICANTx_T;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // ICAN_TX_H
