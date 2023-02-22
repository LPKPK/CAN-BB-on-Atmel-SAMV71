/** @file bb_elem.h
*
* @brief Blackboard element definition
*
* @par
* Copyright (c) 2021 Sunrise Labs, Inc.
* All rights reserved.
*/
#ifndef BB_ELEM_H
#define BB_ELEM_H

#include "can_msg_info.h"
#include <stdint.h>
#include "can_spec.h"

#if defined(__cplusplus)
extern "C"
{
#endif

#define NUM_WORDS_PER_CAN_MSG 2

typedef struct
{
    /// Messaging parameters
    uint32_t dataWords[NUM_WORDS_PER_CAN_MSG];

    /// Update tick of the last time this element was updated.  Helps determine if messages are
    /// stale.
    uint32_t lastUpdate_tick;

    /// Amount of time for the associated CAN message to wait before transmitting.  Used to aid the
    /// CAN Tx scheduler task.
    uint32_t wakeupOffset;
} BlackboardElement_T;

void bb_elem_init(BlackboardElement_T const * bbElement);

/**
 * @brief Getter for the tick count of the last update to the element data
 *
 * @param bbElement Pointer to the blackboard element struct to return the data from
 *
 * @return The number of ticks since the last update
 */
static inline uint32_t bb_elem_get_last_update_ticks(BlackboardElement_T * bbElement)
{
    return bbElement->lastUpdate_tick;
}

/**
 * @brief Setter for the wakeup offset for the element
 *
 * @param bbElement Pointer to the blackboard element struct to set the data of
 */
static inline void bb_elem_set_wakeup_offset(BlackboardElement_T * bbElement, const uint32_t wakeupOffset)
{
    bbElement->wakeupOffset = wakeupOffset;
}

bool_T bb_elem_decrement_wakeup_offset(BlackboardElement_T * bbElement, const uint32_t elapsedTicks);

/**
 * @brief Calculates the number of ticks over the offset, given a measured number of elapsed ticks
 *
 * @param bbElement Pointer to the blackboard element struct
 * @param elapsedTicks Integer containing the number of ticks that have elapsed
 *
 * @return The difference between the ticks elapsed and the wakeup offset of the given blackboard element
 */
static inline uint32_t bb_elem_compute_overflow_ticks(BlackboardElement_T const * bbElement, const uint32_t elapsedTicks)
{
    return elapsedTicks - bbElement->wakeupOffset;
}

/**
 * @brief Calculates the minimum offset ticks based on the wakeup offset and the
 * current minimum. If the wakeup offset of the element is smaller, returns the
 * offset - otherwise, returns the given minimum.
 *
 * @param bbElement Pointer to the blackboard element struct
 * @param minimumOffsetTicks Integer representing the current minimum offset ticks
 *
 * @return The smaller of the current minimum offset and the blackboard element's wakeup offset
 */
static inline uint32_t bb_elem_compute_minimum_offset_ticks(BlackboardElement_T const * bbElement, const uint32_t minimumOffsetTicks)
{
    return (minimumOffsetTicks > bbElement->wakeupOffset) ? bbElement->wakeupOffset : minimumOffsetTicks;
}


void bb_elem_set_uint8_data(BlackboardElement_T * bbElement, const uint8_t data8, const uint8_t index);
void bb_elem_set_int8_data(BlackboardElement_T * bbElement, const int8_t data8, const uint8_t index);
void bb_elem_set_uint16_data(BlackboardElement_T * bbElement, const uint16_t data16, const uint8_t index);
void bb_elem_set_int16_data(BlackboardElement_T * bbElement, const int16_t data16, const uint8_t index);
void bb_elem_set_uint32_data(BlackboardElement_T * bbElement, const uint32_t data32, const uint8_t index);
void bb_elem_set_int32_data(BlackboardElement_T * bbElement, const int32_t data32, const uint8_t index);

uint8_t  bb_elem_get_uint8_data (BlackboardElement_T const * bbElement, const uint8_t index);
int8_t   bb_elem_get_int8_data  (BlackboardElement_T const * bbElement, const uint8_t index);
uint16_t bb_elem_get_uint16_data(BlackboardElement_T const * bbElement, const uint8_t index);
int16_t  bb_elem_get_int16_data (BlackboardElement_T const * bbElement, const uint8_t index);
uint32_t bb_elem_get_uint32_data(BlackboardElement_T const * bbElement, const uint8_t index);
int32_t  bb_elem_get_int32_data (BlackboardElement_T const * bbElement, const uint8_t index);

void bb_elem_get_data_for_can(BlackboardElement_T const * bbElement, uint32_t * const data0, uint32_t * const data1);
void bb_elem_set_data_from_can(BlackboardElement_T * bbElement, const CANElementIndex_T idx,
    const uint32_t data0, const uint32_t data1);

void bb_elem_update_last_tick_count(BlackboardElement_T * bbElement);

#if defined(__cplusplus)
}
#endif

#endif // BB_ELEM
