/** @file can_elem.h
*
* @brief Basic CAN data structure.
*
* @par
* Copyright (c) 2021 Sunrise Labs, Inc.
* All rights reserved.
*/

#ifndef CAN_ELEM_H
#define CAN_ELEM_H

#include "can_msg_info.h"
#include "can_callback.h"
#include "can_chan.h"

#if defined(__cplusplus)
extern "C"
{
#endif

typedef struct
{
    const CANMsgInfo_T canMsgInfo;
    CANCallback_T callback;
    const CanChannel_T txChan;

    /// @todo Rx Channel should be a boolean, not enum
    const CanChannel_T rxChan;
} CANElement_T;

#if defined(__cplusplus)
}
#endif

// can_elem.h and can_spec.h have a carefully crafted inter-dependency since they are really one
// file auto-generated in one half and fixed code in the other. Take care about changing the
// order/location of includes.
#include "can_spec.h"

#if defined(__cplusplus)
extern "C"
{
#endif

const CANElement_T * can_elem_get_can_elem_idx(const CANElementIndex_T idx);

const CANMsgInfo_T * can_elem_get_can_msg_info_idx(const CANElementIndex_T idx);
const CANMsgInfo_T * can_elem_get_can_msg_info_msgid(const uint32_t msgID, CANElementIndex_T *foundIdx);


#if defined(__cplusplus)
}
#endif

#endif // CAN_ELEM_H
