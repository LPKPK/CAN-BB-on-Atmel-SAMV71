/*
 * CanDriver.h
 *
 * Created: 2/17/2023 9:41:51 AM
 *  Author: Pengkai Lyu
 */ 


#ifndef CANDRIVER_H_
#define CANDRIVER_H_

#include <stdint.h>
#include <stdbool.h>

//#include ""

/** CAN data Frame */
typedef struct
{
    uint32_t          id;                                   ///< CAN id.
    uint8_t           data_length_code;                     ///< CAN Data Length code, number of bytes in the message.
    uint8_t           data[8];                              ///< CAN data, up to 8 bytes.
//      can_frame_type_t  type;                             ///< Frame type, data or remote frame.
} can_frame_t;


void can_init();

void candriver_error_poll(void);
bool candriver_read(can_frame_t * msg);
bool candriver_send(uint32_t msg_id, uint8_t *msg_data, uint32_t data_byte_length);

#endif /* CANDRIVER_H_ */