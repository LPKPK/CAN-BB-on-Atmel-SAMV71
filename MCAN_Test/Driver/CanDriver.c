/** @file CanDriver.c
 *
 *  @brief The implementation file for the methods contained in the
 *  CanDriver class.
 *
 *  @par
 *   Copyright © 2021 Sunrise Labs, Inc.
 *   All rights reserved.
 */

#include <stdint.h>
#include <stdbool.h>
#include "CanDriver.h"
#include "qs_mcan_basic.h"


void candriver_error_poll(void)
{
  // CAN1_Tasks();
}

// /**
//  *  @brief The CAN API read function that can be called by users of the driver
//  *  to receive a message.
//  *
//  *  @param msg[out] - the reference to a CAN message object to store received message
//  *
//  *  @return true if message received, false if no message or error.
//  */
// bool candriver_read(can_frame_t *msg)
// {
//   
//   return results;
// }


/**
 *  @brief The CAN API send function that can be called by users of the driver
 *  to send a message.
 *
 *  @pre CAN is open
 *
 *  @param msg_id - the CAN id of the message
 *  @param msg_data - a pointer of the array of data to send
 *  @param data_byte_length - the number of bytes sending
 *
 *  @post A message is sent if the CAN was not busy at the time of sending.
 */
bool candriver_send(uint32_t msg_id, uint8_t *msg_data, uint32_t data_byte_length)
{
  mcan_send_standard_message(msg_id, msg_data,
					data_byte_length); 
  // return result;
}