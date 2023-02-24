/**
 * \file
 *
 * \brief SAM MCAN basic Quick Start
 *
 * Copyright (c) 2015-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */

/**
 *  \mainpage MCAN example
 *
 *  \par Purpose
 *
 *  This example demonstrates the basic functions of MCAN controller.
 *
 *  \par Requirements
 *
 *  This package can be used with two SAMV71 Xplained Ultra boards.
 *  The CAN1 in two board should be connected externally before running
 *  the example.
 *
 *  \par Description
 *
 *  In this example, one board sends messages over CAN bus to another board.
 *  The CAN message will display on the terminal window.
 *
 *  \par Usage
 *
 *  -# Build the program and download it into the evaluation board.
 *  -# On the computer, open and configure a terminal application
 *     (e.g., HyperTerminal on Microsoft Windows) with these settings:
 *    - 115200 bauds
 *    - 8 bits of data
 *    - No parity
 *    - 1 stop bit
 *    - No flow control
 *  -# Connect CAN1 in two boards.
 *  -# Start the application.
 *  -# Upon startup, the application will output the following lines
 *     on the terminal window.
 *      \code
			"  -- Select the action:\r\n"
			"  0: Set standard filter ID 0: 0x45A, store into Rx buffer. \r\n"
			"  1: Set standard filter ID 1: 0x469, store into Rx FIFO 0. \r\n"
			"  2: Send standard message with ID: 0x45A and 4 byte data 0 to 3. \r\n"
			"  3: Send standard message with ID: 0x469 and 4 byte data 128 to 131. \r\n"
			"  4: Set extended filter ID 0: 0x100000A5, store into Rx buffer. \r\n"
			"  5: Set extended filter ID 1: 0x10000096, store into Rx FIFO 1. \r\n"
			"  6: Send extended message with ID: 0x100000A5 and 8 byte data 0 to 7. \r\n"
			"  7: Send extended message with ID: 0x10000096 and 8 byte data 128 to 135. \r\n"
			"  h: Display menu \r\n\r\n");
 \endcode
 *  -# Press '0' or '1' or '4'  or '5' key in the terminal window to configure one board to
 *     receive CAN message.
 *  -# Press '2' or '3' or '6'  or '7' key in the terminal window to configure another board to
 *     send CAN message. The message will be displayed on the terminal window.
 */

/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include <asf.h>
#include <string.h>
#include <conf_mcan.h>
#include "CanDriver.h"
#include "can_tx_processor.h"
#include "message_ctrl.h"
#include "sample.h"

#include"blackboard.h"


#include "bb_exp.h"

// #define TEST

/*Customized*/
volatile uint32_t    g_tickCount        = 0U;
volatile uint64_t    g_tickTime_ms      = 0UL;
volatile uint32_t    g_tx_tick_count_ms = 0U;


 int main(void)
 {
 	uint8_t key;

 	sysclk_init();
 	board_init();

	// sample_init();
	
	g_tickCount = 0;
	g_tx_tick_count_ms = 0;

 	can_init();    // CAN1_Initialize();
 	CANTxTaskInit();
	
	while(1)
  {
	sample_1();
    // CANRxTask();
    CANTxTask(g_tx_tick_count_ms);
    g_tx_tick_count_ms++;
    CanMessageControl_Task();
    #ifdef TEST
        break;
    #endif
    
  }



 	// doSCMtest2();
 }