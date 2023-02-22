/*
 * scm_can_trans.h
 *
 * Created: 2/17/2023 9:37:13 AM
 *  Author: Pengkai Lyu
 */ 


#ifndef SCM_CAN_TRANS_H_
#define SCM_CAN_TRANS_H_

#include "ican_tx.h"

#ifdef __cplusplus
extern "C" {
#endif

void scm_can_trans(ICANTxCtx_T context, uint32_t msgID, uint8_t * data, uint8_t length);

#ifdef __cplusplus
}
#endif

#endif /* SCM_CAN_TRANS_H_ */