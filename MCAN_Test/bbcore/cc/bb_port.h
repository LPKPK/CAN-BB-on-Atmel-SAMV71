/** @file bb_port.h
 *
 * @brief Blackboard Port header file
 *
 * @par
 * Copyright (c) 2021 Sunrise Labs, Inc.
 * All rights reserved.
 */

#ifndef BB_PORT_H
#define BB_PORT_H

#include "bb_types.h"
#include "bb_elem.h"

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * @brief lightweight porting interface for the Blackboard to run in different environments.
 *
 * Implement these functions in your environment, e.g. FreeRTOS, Linux/Qt, bare metal
 */

//lint -save -e1526 undefined member - defined elsewhere
/**
 * Determines if the port is running in an ISR context.
 * @return true if the port is in an ISR context
 */
//lint -e{1714} member function not referenced
// TODO: Deal with this
bool_T bb_port_is_ISR_context(void);

/**
 * Determines if the port is running in scheduler mode, or if the scheduler has not yet
 * started.  May not be applicable.
 * @return true if the scheduler is running, false otherwise.
 */
bool_T bb_port_is_scheduler_running(void);

/**
 * Returns the current time in ticks from the port.
 * @return the current time in ticks
 */
uint32_t bb_port_get_current_ticks(void);

/**
 * Sleeps for the given time in ms
 * @param sleepTimeMs the amount of time to sleep in ms
 */
void bb_port_sleep_ticks(uint32_t sleepTimeTicks);

/**
 * Sets data directly within a BlackboardElement.  This may be implemented using intrinsic atomics,
 * c++ generic atomics, or some other mechanism such as mutexes.
 *
 * @param element A BlackboardElement in which the port can set dataWords.
 * @param data8 input uint8_t value
 * @param byteIndexInElement The index relative to an 8 byte CAN message payload, ranging from 0-7
 */
void bb_port_set_data_direct8(BlackboardElement_T * element, const uint8_t data8, const uint8_t byteIndexInElement);

/**
 * Sets data directly within a BlackboardElement.  This may be implemented using intrinsic atomics,
 * c++ generic atomics, or some other mechanism such as mutexes.
 *
 * @param element A BlackboardElement in which the port can set dataWords.
 * @param data16 input uint16_t value
 * @param byteIndexInElement The index relative to an 8 byte CAN message payload, ranging from 0-7
 */
void bb_port_set_data_direct16(BlackboardElement_T * element, const uint16_t data16, const uint8_t byteIndexInElement);

/**
 * Sets data directly within a BlackboardElement.  This may be implemented using intrinsic atomics,
 * c++ generic atomics, or some other mechanism such as mutexes.
 *
 * @param element A BlackboardElement in which the port can set dataWords.
 * @param data32 input uint32_t value
 * @param byteIndexInElement The index relative to an 8 byte CAN message payload, ranging from 0-7
 */
void bb_port_set_data_direct32(BlackboardElement_T * element, const uint32_t data32, const uint8_t byteIndexInElement);

/**
 * Gets data directly atomically within the given BlackboardElement.
 *
 * @param element A BlackboardElement in which the port can set dataWords.
 * @param byteIndexInElement The index relative to an 8 byte CAN message payload, ranging from 0-7
 * @return The u8 value read at the given byte index
 */
uint8_t bb_port_get_data_direct8(const BlackboardElement_T * element, const uint8_t byteIndexInElement);

/**
 * Gets data directly atomically within the given BlackboardElement.
 *
 * @param element A BlackboardElement in which the port can set dataWords.
 * @param byteIndexInElement The index relative to an 8 byte CAN message payload, ranging from 0-7
 * @return The u16 value read at the given byte index
 */
uint16_t bb_port_get_data_direct16(const BlackboardElement_T * element, const uint8_t byteIndexInElement);

/**
 * Gets data directly atomically within the given BlackboardElement.
 *
 * @param element A BlackboardElement in which the port can set dataWords.
 * @param byteIndexInElement The index relative to an 8 byte CAN message payload, ranging from 0-7
 * @return The u32 value read at the given byte index
 */
uint32_t bb_port_get_data_direct32(const BlackboardElement_T * element, const uint8_t byteIndexInElement);

//lint -restore

#if defined(__cplusplus)
}
#endif

#endif /* BB_PORT_H */
