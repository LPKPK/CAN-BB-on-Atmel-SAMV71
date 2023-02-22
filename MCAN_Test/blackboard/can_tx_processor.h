/*
 * can_tx_processor.h
 *
 * Created: 2/21/2023 9:38:34 AM
 *  Author: Pengkai Lyu
 */ 


#ifndef CAN_TX_PROCESSOR_H_
#define CAN_TX_PROCESSOR_H_

#include "bb_tx_proc.h"

void CANTxTaskInit(void);
void CANTxTask(uint32_t curr_tick_ms);


#endif /* CAN_TX_PROCESSOR_H_ */