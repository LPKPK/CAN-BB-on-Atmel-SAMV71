/** @file bb_elem.c
*
* @brief Blackboard Element base struct and function declarations.
*
*   Holds blackboard data and provides access to information
*   Each Blackboard element structure matches the CAN message structure
*
* @par
* Copyright (c) 2021 Sunrise Labs, Inc.
* All rights reserved.
*/
#include "bb_elem.h"
#include "bb_port.h"
#include "bb_utils.h"


#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * @brief Decrements the element's wakeup offset if the elapsed ticks are less than the offset.
 * Otherwise, returns false and does not change the offset.
 *
 * @param elapsedTicks Ticks elapsed since the last transmission
 *
 * @return True if the wakeup offset was decremented, false if it was not
 */
bool_T bb_elem_decrement_wakeup_offset(BlackboardElement_T * bbElement, const uint32_t elapsedTicks)
{
    bool_T decremented = false;
    if (bbElement->wakeupOffset > elapsedTicks)
    {
        bbElement->wakeupOffset = bbElement->wakeupOffset - elapsedTicks;
        decremented = true;
    }
    return decremented;
}

/**
 * @brief Store data in the element buffer
 *
 * @param data8 uint8_t value to store
 * @param index index into buffer of where to store data
 */
void bb_elem_set_uint8_data(BlackboardElement_T * bbElement, const uint8_t data8, const uint8_t index)
{
    bb_port_set_data_direct8(bbElement, data8, index);

    bb_elem_update_last_tick_count(bbElement);
}

/**
 * @brief Store data in the element buffer
 *
 * @param data16 uint16_t value to store
 * @param index index into buffer of where to store data
 */
void bb_elem_set_uint16_data(BlackboardElement_T * bbElement, const uint16_t data16, const uint8_t index)
{
    bb_port_set_data_direct16(bbElement, data16, index);

    bb_elem_update_last_tick_count(bbElement);
}

/**
 * @brief Store data in the element buffer
 *
 * @param data32 uint32_t value to store
 * @param index index into buffer of where to store data
 */
void bb_elem_set_uint32_data(BlackboardElement_T * bbElement, const uint32_t data32, const uint8_t index)
{
    bb_port_set_data_direct32(bbElement, data32, index);

    bb_elem_update_last_tick_count(bbElement);
}

/**
 * @brief Store data in the element buffer
 *
 * @param data8 int8_t value to store
 * @param index index into buffer of where to store data
 */
void bb_elem_set_int8_data(BlackboardElement_T * bbElement, const int8_t data8, const uint8_t index)
{
    bb_elem_set_uint8_data(bbElement, bb_utils_toUnsigned8(data8), index);
}

/**
 * @brief Store data in the element buffer
 *
 * @param data16 int16_t value to store
 * @param index index into buffer of where to store data
 */
void bb_elem_set_int16_data(BlackboardElement_T * bbElement, const int16_t data16, const uint8_t index)
{
    bb_elem_set_uint16_data(bbElement, bb_utils_toUnsigned16(data16), index);
}

/**
 * @brief Store data in the element buffer
 *
 * @param data32 int32_t value to store
 * @param index index into buffer of where to store data
 */
void bb_elem_set_int32_data(BlackboardElement_T * bbElement, const int32_t data32, const uint8_t index)
{
    bb_elem_set_uint32_data(bbElement, bb_utils_toUnsigned32(data32), index);
}

/**
 * @brief Get typed data from data buffer
 *
 * @param index index into buffer of where to get data
 *
 * @return acquired uint8_t data
 */
uint8_t bb_elem_get_uint8_data(BlackboardElement_T const * bbElement, const uint8_t index)
{
    return bb_port_get_data_direct8(bbElement, index);
}

/**
 * @brief Get typed data from data buffer
 *
 * @param index index into buffer of where to get data
 *
 * @return acquired int8_t data
 */
int8_t bb_elem_get_int8_data(BlackboardElement_T const * bbElement, const uint8_t index)
{
    return bb_utils_toSigned2sComp8(bb_elem_get_uint8_data(bbElement, index));
}

/**
 * @brief Get typed data from data buffer
 *
 * @param index index into buffer of where to get data
 *
 * @return acquired uint16_t data
 */
uint16_t bb_elem_get_uint16_data(BlackboardElement_T const * bbElement, const uint8_t index)
{
    return bb_port_get_data_direct16(bbElement, index);
}

/**
 * @brief Get typed data from data buffer
 *
 * @param index index into buffer of where to get data
 *
 * @return acquired int16_t data
 */
int16_t bb_elem_get_int16_data(BlackboardElement_T const * bbElement, const uint8_t index)
{
    return bb_utils_toSigned2sComp16(bb_elem_get_uint16_data(bbElement, index));
}

/**
 * @brief Get typed data from data buffer
 *
 * @param index index into buffer of where to get data
 *
 * @return acquired uint32_t data
 */
uint32_t bb_elem_get_uint32_data(BlackboardElement_T const * bbElement, const uint8_t index)
{
    return bb_port_get_data_direct32(bbElement, index);
}

/**
 * @brief Get typed data from data buffer
 *
 * @param index index into buffer of where to get data
 *
 * @return acquired int32_t data
 */
int32_t bb_elem_get_int32_data(BlackboardElement_T const * bbElement, const uint8_t index)
{
    return bb_utils_toSigned2sComp32(bb_elem_get_uint32_data(bbElement, index));
}

/**
 * @brief Retrieves the internal 8 bytes of data within this element, presumably for transmission
 * out the CAN bus.
 *
 * Always copies two 32 bit words regardless of the data-length of the message
 *
 * @param[out] data0 a pointer to a u32 that will be copied with the lower 4 bytes of data.
 * @param[out] data1 a pointer to a u32 that will be copied with the upper 4 bytes of data.
 */
void bb_elem_get_data_for_can(BlackboardElement_T const * bbElement, uint32_t * const data0, uint32_t * const data1)
{
    *data0 = bb_port_get_data_direct32(bbElement, 0U);
    *data1 = bb_port_get_data_direct32(bbElement, 4U);
}

/**
 * @brief Sets the internal 8 bytes of data within this element, presumably for reception from the
 * CAN bus.
 *
 * Always copies two 32 bit words regardless of the data-length of the message
 *
 * @param bbElement Pointer to the blackboard element struct whose data is being set
 * @param idx The index of the can element to get the callback from - to conserve memory, this
 *      is passed in rather than stored in the blackboard element struct.
 * @param data0 a u32 that will be copied into the lower 4 bytes of this element's data.
 * @param data1 a u32 that will be copied into the upper 4 bytes of this element's data.
 */
void bb_elem_set_data_from_can(BlackboardElement_T * bbElement, const CANElementIndex_T idx,
    const uint32_t data0, const uint32_t data1)
{
    bb_port_set_data_direct32(bbElement, data0, 0U);
    bb_port_set_data_direct32(bbElement, data1, 4U);

    const CANElement_T * canElement = can_elem_get_can_elem_idx(idx);

    if (canElement->callback.callbackFn != NULL)
    {
        canElement->callback.callbackFn(canElement->callback.callbackCtx,
                                         CAN_NONE,              // Tx Channel is N/A
                                         canElement->rxChan);   // Rx Channel
    }
}

/**
 * @brief updateLastTickCount updates last tick count to the current tick if the scheduler is running
 *
 * @return none
 */
void bb_elem_update_last_tick_count(BlackboardElement_T * bbElement)
{
    bbElement->lastUpdate_tick = bb_port_get_current_ticks();
}

#if defined(__cplusplus)
}
#endif
