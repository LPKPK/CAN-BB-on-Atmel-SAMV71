/** @file CanDriver.c
 *
 *  @brief The implementation file for the methods contained in the
 *  CanDriver class.
 *
 *  @par
 *   Copyright © 2021 Sunrise Labs, Inc.
 *   All rights reserved.
 */

#include <stdint.h>
#include <asf.h>
#include <string.h>
#include <conf_mcan.h>

/* module_inst */
static struct mcan_module mcan_instance;



/* mcan_transfer_message_setting */
#define MCAN_TX_BUFFER_INDEX    0
static uint8_t tx_message_0[CONF_MCAN_ELEMENT_DATA_SIZE];
static uint8_t tx_message_1[CONF_MCAN_ELEMENT_DATA_SIZE];




/**
 * \brief Configure UART console.
 */
static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
#ifdef CONF_UART_CHAR_LENGTH
		.charlength = CONF_UART_CHAR_LENGTH,
#endif
		.paritytype = CONF_UART_PARITY,
#ifdef CONF_UART_STOP_BITS
		.stopbits = CONF_UART_STOP_BITS,
#endif
	};

	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

/**
 * \brief MCAN module initialization.
 *
 */
static void configure_mcan(void)
{
	uint32_t i;
	/* Initialize the memory. */
	for (i = 0; i < CONF_MCAN_ELEMENT_DATA_SIZE; i++) {
		tx_message_0[i] = i;
		tx_message_1[i] = i + 0x80;
	}

	/* Initialize the module. */
	struct mcan_config config_mcan;
	mcan_get_config_defaults(&config_mcan);
	mcan_init(&mcan_instance, MCAN_MODULE, &config_mcan);


	mcan_start(&mcan_instance);
	/* Enable interrupts for this MCAN module */
	irq_register_handler(MCAN1_INT0_IRQn, 1);
	mcan_enable_interrupt(&mcan_instance, MCAN_FORMAT_ERROR | MCAN_ACKNOWLEDGE_ERROR | MCAN_BUS_OFF);
}

void can_init()
{
	configure_console();
	configure_mcan();
}


/**
 * \brief send standard MCAN message,
 *
 *\param id_value standard MCAN ID
 *\param *data  content to be sent
 *\param data_length data length code
 */
static void mcan_send_standard_message(uint32_t id_value, uint8_t *data,
		uint32_t data_length)
{
	uint32_t i;
	struct mcan_tx_element tx_element;

	mcan_get_tx_buffer_element_defaults(&tx_element);
	tx_element.T0.reg |= MCAN_TX_ELEMENT_T0_STANDARD_ID(id_value);
	tx_element.T1.bit.DLC = data_length;
	for (i = 0; i < data_length; i++) {
		tx_element.data[i] = *data;
		data++;
	}

	mcan_set_tx_buffer_element(&mcan_instance, &tx_element,
			MCAN_TX_BUFFER_INDEX);
	mcan_tx_transfer_request(&mcan_instance, 1 << MCAN_TX_BUFFER_INDEX);
}

/**
 * \brief send extended MCAN message,
 *
 *\param id_value extended MCAN ID
 *\param *data  content to be sent
 *\param data_length data length code
 */
static void mcan_send_extended_message(uint32_t id_value, uint8_t *data,
		uint32_t data_length)
{
	uint32_t i;
	struct mcan_tx_element tx_element;

	mcan_get_tx_buffer_element_defaults(&tx_element);
	tx_element.T0.reg |= MCAN_TX_ELEMENT_T0_EXTENDED_ID(id_value) |
			MCAN_TX_ELEMENT_T0_XTD;
	tx_element.T1.bit.DLC = data_length;
	for (i = 0; i < data_length; i++) {
		tx_element.data[i] = *data;
		data++;
	}

	mcan_set_tx_buffer_element(&mcan_instance, &tx_element,
			MCAN_TX_BUFFER_INDEX);
	mcan_tx_transfer_request(&mcan_instance, 1 << MCAN_TX_BUFFER_INDEX);
}

/**
 * \brief Interrupt handler for MCAN,
 *   inlcuding RX,TX,ERROR and so on processes.
 */
void MCAN1_INT0_Handler(void)
{
	volatile uint32_t status, i, rx_buffer_index;
	status = mcan_read_interrupt_status(&mcan_instance);


	if (status & MCAN_BUS_OFF) {
		mcan_clear_interrupt_status(&mcan_instance, MCAN_BUS_OFF);
		mcan_stop(&mcan_instance);
		printf(": MCAN bus off error, re-initialization. \r\n\r\n");
		configure_mcan();
	}

	if (status & MCAN_ACKNOWLEDGE_ERROR) {
		mcan_clear_interrupt_status(&mcan_instance, MCAN_ACKNOWLEDGE_ERROR);
		printf("Protocol ACK error, please double check the clock in two boards. \r\n\r\n");
	}

	if (status & MCAN_FORMAT_ERROR) {
		mcan_clear_interrupt_status(&mcan_instance, MCAN_FORMAT_ERROR);
		printf("Protocol format error, please double check the clock in two boards. \r\n\r\n");
	}
}


void candriver_error_poll(void)
{
//   CAN1_Tasks();
}


/**
 *  @brief The CAN API send function that can be called by users of the driver
 *  to send a message.
 *
 *  @pre CAN is open
 *
 *  @param msg_id - the CAN id of the message
 *  @param msg_data - a pointer of the array of data to send
 *  @param data_byte_length - the number of bytes sending
 *
 *  @post A message is sent if the CAN was not busy at the time of sending.
 */
bool candriver_send(uint32_t msg_id, uint8_t *msg_data, uint32_t data_byte_length)
{
  mcan_send_standard_message(msg_id, msg_data,
					data_byte_length); 
  // return result;
}