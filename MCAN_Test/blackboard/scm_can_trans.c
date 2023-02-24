/*
 * scm_can_trans.c
 *
 * Created: 2/17/2023 9:36:54 AM
 *  Author: Pengkai Lyu
 */ 

/** @file gdn_can_transmitter.c
*
* @brief Function definitions for transmission of CAN messages
*
* @par
* Copyright (c) 2021 Sunrise Labs, Inc.
* All rights reserved.
*/

#include "scm_can_trans.h"
#include "sli_assert.h"
#include "CanDriver.h"
#include "stdio.h"


/**
* @brief GDN implementation of the ican_tx interface
*
* @param context A pointer to the CanDriver that was configured to send CAN messages
* @param msgID The CAN message ID to send
* @param data CAN payload data, update to 8 bytes
* @param length Number of CAN data payload bytes to send
*/

void scm_can_trans(ICANTxCtx_T context, uint32_t msgID, uint8_t * data, uint8_t length)
{
	const uint8_t MaxBytesPerCanMsg = 8U; // TODO: Centralized definition for this?
	SLI_ASSERT(length <= MaxBytesPerCanMsg);
	
	(void)context;    // Unused

	// CANDriver can track its own error counter
    (void)candriver_send(msgID, data, length);

	candriver_error_poll();
	printf("Success");
	
}