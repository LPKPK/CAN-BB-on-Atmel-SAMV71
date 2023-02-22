/** @file canchannel.h
 *
 * @brief Defines valid CAN Channels for messages.
 *
 * @par
 * Copyright (c) 2021 Sunrise Labs, Inc.
 * All rights reserved.
 */

#ifndef CAN_CHAN_H
#define CAN_CHAN_H

typedef enum
{
    CAN_NONE  = 0,
    CAN_CHAN1 = 1,
    CAN_CHAN2 = 2,
    CAN_CHAN3 = 4,
} CanChannel_T;

#endif // CAN_CHAN_H
