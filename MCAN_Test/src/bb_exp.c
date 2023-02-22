/*
 * bb_exp.c
 *
 * Created: 2/16/2023 4:25:33 PM
 *  Author: Pengkai Lyu
 */ 
#include <asf.h>
#include "blackboard.h"
#include "can_elem.h"
#include "ican_tx.h"
#include "bb_port.h"
#include "CanDriver.h"
// #include "scm_can_trans.h"

#include "sli_assert.h"


static uint32_t prevTicks;
static uint32_t sleepTicks;

//uint32_t bb_port_get_current_tick();

// void iCanHazTx(ICANTxCtx_T context, uint32_t msgID, uint8_t * data, uint8_t length)  //scm_can_transmitter()
// {
// 	// uint32_t currTick = bb_port_get_current_tick();

// 	// char dataStr[(8 * 3) + 1];
// 	// size_t startPtr = 0;
// 	// size_t maxBytes = sizeof(dataStr);
// 	// for (int i = 0; i < length; i++)
// 	// {
// 	// 	startPtr += snprintf(dataStr + startPtr, maxBytes, "%02x ", data[i]);
// 	// }
	
// 	const uint8_t MaxBytesPerCanMsg = 8U; // TODO: Centralized definition for this?
// 	SLI_ASSERT(length <= MaxBytesPerCanMsg);
  
// 	(void)context;    // Unused
  
//     // CANDriver can track its own error counter
// 	//(void)mcan_send_standard_message(msgID, data, length);
// 	printf("SCM: iCanHazTx: %p, 0x%x, %d\n", context, msgID, data);
// 	// candriver_error_poll();
// }

void iCanHazTx(ICANTxCtx_T context, uint32_t msgID, uint8_t * data, uint8_t length)
{
    uint32_t currTick = bb_port_get_current_ticks();
	/*Initialization
		TBD
	*/


        // startPtr += snprintf(dataStr + startPtr, maxBytes, "%02x ", data[i]);
		(void)candriver_send(msgID, data, length);

    printf("SCM: iCanHazTx: %d: %p, 0x%x: %s\n", currTick, context, msgID, data);
}

extern volatile uint32_t g_tx_tick_count_ms;

void doSCMtest1(void)
{
	int hello = 42;
	printf("Hello RCM %d\n", hello);

	//foo = 43;

	uint8_t data[] = { 10, 11, 12, 13};

	iCanHazTx((ICANTxCtx_T)42, 0x402, data, sizeof(data));

	for (int i = 0; i < 10; i++)
	{
		uint32_t initTick = bb_port_get_current_ticks();
		int tick;
		do
		{
			tick = bb_port_get_current_ticks();
			g_tx_tick_count_ms++;
			// printf("cticks %d\n", tick);
		} while((tick - initTick) < 1000);

		iCanHazTx((ICANTxCtx_T)42, 0x400 + i, data, sizeof(data));
	}


	CANElement_T ce = can_spec_can_element_list[CAN_ELEM_SCM_SystemBarfy];
	printf("id = 0x%x, name=%s, period_ms=%d, numBytes=%d\n",
	ce.canMsgInfo.id,
	ce.canMsgInfo.name,
	ce.canMsgInfo.period_ms,
	ce.canMsgInfo.numBytes);

	if (ce.callback.callbackFn != NULL)
	{
		ce.callback.callbackFn(ce.callback.callbackCtx, ce.rxChan, ce.txChan);
		ce.callback.callbackFn(ce.callback.callbackCtx, ce.rxChan, ce.txChan);
	}

}

#include "bb_tx_proc.h"

void bb_port_inc_ticks();

void doSCMtest2(void)
{
	BlackboardTxProcessor_T bbtxProc;

	static ICANTx_T icanHazTxers[1] =
	{
		{ &iCanHazTx, NULL }
	};

	// bb_set_RCM_ModuleId(42);
	// bb_set_RCM_Sensor1Voltage(3.5);
	// bb_set_RCM_Sensor1Temperature(4095.5);

	bb_set_SCM_motor1MaxSpeed(4095);
	bb_set_SCM_motor1MinSpeed(5072);
	bb_set_SCM_Motor1Control(42);

	uint32_t currTicks = 0; //bb_port_get_current_tick();
	uint32_t prevTicks = currTicks;
	bb_tx_proc_init(&bbtxProc, 10000, icanHazTxers, sizeof(icanHazTxers)/sizeof(icanHazTxers[0]), currTicks);

	for (int i = 0; i < 100; i++)
	{
		currTicks = bb_port_get_current_ticks();
		uint32_t sleepTicks = bb_tx_proc_process_tx(&bbtxProc, currTicks, prevTicks);
		if (sleepTicks > 200)
		{
			sleepTicks = 200;
		}
		prevTicks = currTicks;
		bb_port_inc_ticks(sleepTicks);
		 g_tx_tick_count_ms++;
	}

	BlackboardElement_T * bbElement = bb_get_element(CAN_ELEM_RCM_System);
	bb_elem_set_data_from_can(bbElement, CAN_ELEM_RCM_System, 0xDECAFC0F, 0xFEEBEBAD);
	// bb_elem_get_data_for_can(bbElement, &tx_data[0], &tx_data[1]);
}