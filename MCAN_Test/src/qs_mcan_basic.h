/*
 * qs_mcan_basic.h
 *
 * Created: 2/21/2023 11:57:35 AM
 *  Author: Pengkai Lyu
 */ 


#ifndef QS_MCAN_BASIC_H_
#define QS_MCAN_BASIC_H_

void mcan_send_standard_message(uint32_t id_value, uint8_t *data, uint32_t data_length);



#endif /* QS_MCAN_BASIC_H_ */