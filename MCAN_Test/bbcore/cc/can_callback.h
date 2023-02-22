/** @file cancallback.h
*
* @brief Abstract definition for functor to be called on CAN event.
*
* @par
* Copyright (c) 2021 Sunrise Labs, Inc.
* All rights reserved.
*/

#ifndef CAN_CALLBACK_H
#define CAN_CALLBACK_H

#include "can_chan.h"

#if defined(__cplusplus)
extern "C"
{
#endif

typedef void * CANCallbackCtx_T;

/// @brief The Rx and Tx channels are sent into the callback function so that the code there can determine whether
/// the callback is being called on transmit or receive.
/// @todo rxchannel should be a flag, not a bit mask
typedef void CANCallbackFn_T(CANCallbackCtx_T context, CanChannel_T rxChan, CanChannel_T txChan);

typedef struct
{
    CANCallbackFn_T * callbackFn;
    CANCallbackCtx_T callbackCtx;
} CANCallback_T;


#if defined(__cplusplus)
}
#endif

#endif // CAN_CALLBACK_H
