/** @file can_msg_info.h
*
* @brief Info about each can message
*
* @par
* Copyright (c) 2021 Sunrise Labs, Inc.
* All rights reserved.
*/
#ifndef CAN_MSG_INFO_H
#define CAN_MSG_INFO_H

#include "bb_types.h"

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

#define MAX_NUM_DATA_ITEMS (4U)

typedef bool_T ValueSign_T;

#define NULL_STR      (NULL)

#define CAN_DATA_SIGNED true        //lint !e755 global macro not referenced
#define CAN_DATA_UNSIGNED false

//lint -e{768} global structure member not referenced - these are used, Lint can't seem to see them
typedef struct
{
    const char_T* const name;
    ValueSign_T signed_val;
    uint8_t start_byte;
    uint16_t byte_count; // u16 to pad
} CANDataInfo_T;

//lint -e{768} global structure member not referenced - these are used, Lint can't seem to see them
//lint -e{9150} non-private data member within a non-POD structure - Lint is wrong here
typedef struct
{
    const uint32_t id;
    const char_T* const name;
    const uint32_t period_ms;
    const uint8_t numBytes;
    const CANDataInfo_T data_info[MAX_NUM_DATA_ITEMS];
} CANMsgInfo_T;


#if defined(__cplusplus)
}
#endif

#endif // CAN_MSG_INFO_H
