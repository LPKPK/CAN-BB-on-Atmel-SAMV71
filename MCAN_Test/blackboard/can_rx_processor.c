#include "CanDriver.h"

#include "can_rx_processor.h"

#include "can_elem.h"
#include "bb_elem.h"
#include "blackboard.h"

#include "stdio.h"

static inline uint32_t u8s_to_32(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3)
{
  return
    (uint32_t)b0 +
    ((uint32_t)b1 * 256U) +
    ((uint32_t)b2 * 256U * 256U) +
    ((uint32_t)b3 * 256U * 256U * 256U);
}

// #include "stdint.h"
static int16_t canMessageNotReceived;

void CANRxTask(void)
{
  static const int16_t NO_CAN_MESSAGE_RECEIVED = 12000;   // 1 minute
  can_frame_t r_message;
  (void)candriver_read(&r_message);
  CANElementIndex_T foundIdx = CAN_ELEM_LAST_MSG;
  (void)can_elem_get_can_msg_info_msgid(r_message.id, &foundIdx);
  if (foundIdx != CAN_ELEM_LAST_MSG){
    printf("%d\r\n", foundIdx);
    // Find the corresponding Blackboard Element using the index derived from the message ID
      const CANElement_T * canElement = can_elem_get_can_elem_idx(foundIdx);
      if ((uint32_t)(canElement->rxChan) != 0UL)
      {
        canMessageNotReceived = 0; // We got a message
        // Convert the 8-byte array into two 32-bit values
        const uint32_t d0 = u8s_to_32(r_message.data[0], r_message.data[1], r_message.data[2], r_message.data[3]);
        const uint32_t d1 = u8s_to_32(r_message.data[4], r_message.data[5], r_message.data[6], r_message.data[7]);

        // Update the data in the Blackboard
        BlackboardElement_T *bbElement = bb_get_element(foundIdx);
        bb_elem_set_data_from_can(bbElement, foundIdx, d0, d1);
      }
  }


  // if (candriver_read(&r_message))
  // {
  //     printf("HazMsg");
  //   CANElementIndex_T foundIdx = CAN_ELEM_LAST_MSG;
  //   (void)can_elem_get_can_msg_info_msgid(r_message.id, &foundIdx);
  //   canMessageNotReceived++;   // Let's assume no valid message has been received
  //   if (NO_CAN_MESSAGE_RECEIVED <= canMessageNotReceived)
  //   {  // Don't wrap around
  //     canMessageNotReceived = NO_CAN_MESSAGE_RECEIVED;
  //   }

  //   if (foundIdx != CAN_ELEM_LAST_MSG)
  //   {
  //     // Find the corresponding Blackboard Element using the index derived from the message ID
  //     const CANElement_T * canElement = can_elem_get_can_elem_idx(foundIdx);
  //     if ((uint32_t)(canElement->rxChan) != 0UL)
  //     {
  //       canMessageNotReceived = 0; // We got a message
  //       // Convert the 8-byte array into two 32-bit values
  //       const uint32_t d0 = u8s_to_32(r_message.data[0], r_message.data[1], r_message.data[2], r_message.data[3]);
  //       const uint32_t d1 = u8s_to_32(r_message.data[4], r_message.data[5], r_message.data[6], r_message.data[7]);

  //       // Update the data in the Blackboard
  //       BlackboardElement_T *bbElement = bb_get_element(foundIdx);
  //       bb_elem_set_data_from_can(bbElement, foundIdx, d0, d1);
  //     }
  //   }
  //   // else is a message this device does not subscribe to. Not an error.
  // }
}