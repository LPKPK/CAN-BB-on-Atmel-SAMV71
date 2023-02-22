/*
 * CanDriver.h
 *
 * Created: 2/17/2023 9:41:51 AM
 *  Author: Pengkai Lyu
 */ 


#ifndef CANDRIVER_H_
#define CANDRIVER_H_

bool candriver_send(uint32_t msg_id, uint8_t *msg_data, uint32_t data_byte_length);

#endif /* CANDRIVER_H_ */