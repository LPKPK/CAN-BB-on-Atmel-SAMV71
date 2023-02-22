/**  @file can_tx_processor.c
*
*  @brief The implementation file for can message transmission functions
*
*  @par
*  Copyright ? 2021 Sunrise Labs, Inc.
*  All rights reserved.
*/
#include "can_tx_processor.h"
#include "scm_can_trans.h"
#include <samv71.h>
#include "time.h"
#include <stddef.h>
#include <stdio.h>

#define TIME_TICK_FREQUENCY_HZ (uint32_t)(200)
#define TIME_TICK_LIMIT 0xffff
#define MS_TO_TICKS(ms) (TIME_TICK_FREQUENCY_HZ * (ms) / (uint32_t)1000)

static BlackboardTxProcessor_T bbtxProc;

static uint32_t prevTicks;
static uint32_t sleepTicks;

void CANTxTaskInit(void)
{
	static ICANTx_T icanHazTxers[1] =
	{
		{ &scm_can_trans, NULL }
	};

	const uint32_t MaxCanTxProcDelay = 10000UL;
	sleepTicks = 0U;
	prevTicks = 0U;
	bb_tx_proc_init(&bbtxProc, MaxCanTxProcDelay, icanHazTxers, (uint32_t)(sizeof(icanHazTxers)/sizeof(icanHazTxers[0])), 0U);
}

void CANTxTask(uint32_t curr_tick_ms)
{
	if (curr_tick_ms >= (prevTicks + sleepTicks))
	{
		sleepTicks = bb_tx_proc_process_tx(&bbtxProc, curr_tick_ms, prevTicks);
		prevTicks = curr_tick_ms;
	}

	// ThreadX unfortunately does not have the nice vTaskDelayUntil function that FreeRTOS has
	// This sequence works around that, with some possible jitter, but overall should keep
	// the timing of the CAN transmission on a fixed schedule. An alternative would be to have
	// a timer task that fires on a monotonically increasing timer, and have it constantly
	// "chasing" the next time, and each time it fires, have it signal a semaphore instead
	// of sleeping. That might give better results.

	// Save the previous ticks to the current time
	//prevTicks = currTicks;
	// Artificially increment the currTicks monotonically by the prescribed sleepTicks
	//currTicks += sleepTicks;
	// Get the latest actual ticks - should be most often same as prevTicks
	//const uint32_t newCurrTicks = counter;
	// Compute the sleep amount based on the monotonic currTicks and the latest actual time
	// Should almost always be the same as sleepTicks
	//const uint32_t actualSleepTicks = currTicks - newCurrTicks;
	// Perform the sleep
	//(void)tx_thread_sleep(actualSleepTicks);

}