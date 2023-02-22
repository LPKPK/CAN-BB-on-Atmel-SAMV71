/*
 * @file bb_port.c
 *
 * @brief RCM specific implementation of Blackboard related utility functions
 *
 * @par
 * Copyright (c) 2021 Sunrise Labs, Inc. All rights
 * reserved.
 */

#include "bb_port.h"
#include "bb_types.h"
#include "bb_elem.h"
#include "sli_assert.h"
#include <time.h>

extern volatile uint32_t g_tx_tick_count_ms;

#define BB_UNIT_TEST
/*
 * @brief Determines if the calling code is in an ISR context
 * @todo Determine how to implement/if this is needed
 *
 * @return false (placeholder)
 */
bool_T bb_port_is_ISR_context()
{
    return false;

    // TODO
#if 0
    constexpr unsigned long fiqBits = 0x11UL;
    constexpr unsigned long irqBits = 0x12UL;
    volatile const unsigned long cpsr = __get_CPSR();
    return ((cpsr & fiqBits) == fiqBits) || ((cpsr & irqBits) == irqBits);
    return true;
#endif
}

/*
 * @brief Determines if the scheduler is running
 * @todo Determine how to implement/if this is needed
 *
 * @return true (placeholder)
 */
bool_T bb_port_is_scheduler_running()
{
    // TODO: Let's do this differently to make it more portable and not rely on port ASM code.
    // Create a bootstrap task that sets the "scheduler is running" flag to true?
    //return isISRContext() ? xTaskIsSchedulerStartedFromISR() : xTaskIsSchedulerStarted();

    return true;
}
#ifdef BB_UNIT_TEST
static int fakeTick = 0;

void bb_port_inc_ticks(uint32_t ticks)
{
    fakeTick += ticks;
}

void bb_port_set_ticks(uint32_t ticks)
{
    fakeTick = ticks;
}
#endif
/*
 * @brief Returns the current tick count
 *
 * @return The current tick count
 */
uint32_t bb_port_get_current_tick(void)
{
    return fakeTick;
}

/**
 * @brief Returns the current time in ticks from the GDN main loop using 
 * g_tx_tick_count_ms.
 * @return the current time in ticks
 */
uint32_t bb_port_get_current_ticks(void)
{
  return g_tx_tick_count_ms;
}

/**
 * Implementation of BlackboardPort interface that uses ARM load & store-exclusive instructions to
 * perform an atomic write to the internal blackboard element storage.
 *
 * @see BlackboardPort.h for details on this method.
 */
void bb_port_set_data_direct8(BlackboardElement_T * element, const uint8_t data8, const uint8_t byteIndexInElement)
{
    // Get the offset/index into internal _dataWords given logical byte index within the element
    const uint32_t byteOffsetInWord = (uint32_t)(byteIndexInElement) % 4U;
    const uint32_t wordIndex = (uint32_t)(byteIndexInElement) / 4U;

    volatile uint8_t * const dest8Ptr = (volatile uint8_t *)(&element->dataWords[wordIndex]) + byteOffsetInWord;

    *dest8Ptr = data8;
}

/**
 * Implementation of BlackboardPort interface that uses ARM load & store-exclusive instructions to
 * perform an atomic write to the internal blackboard element storage.
 *
 * @see BlackboardPort.h for details on this method.
 */
void bb_port_set_data_direct16(BlackboardElement_T * element, const uint16_t data16, const uint8_t byteIndexInElement)
{
    // Get the offset/index into internal _dataWords given logical byte index within the element
    const uint32_t byteOffsetInWord = (uint32_t)(byteIndexInElement) % 4U;
    const uint32_t wordIndex = (uint32_t)(byteIndexInElement) / 4U;

    // Dereferencing a u16 pointer with a byte offset.  Make sure it is u16-aligned within the
    // element.
    SLI_ASSERT((byteOffsetInWord % 2U) == 0);
    volatile uint16_t * const dest16Ptr = (volatile uint16_t *)(((volatile uint8_t *)&element->dataWords[wordIndex]) + byteOffsetInWord);

    *dest16Ptr = data16;
}

/**
 * Implementation of BlackboardPort interface that uses ARM load & store-exclusive instructions to
 * perform an atomic write to the internal blackboard element storage.
 *
 * @see BlackboardPort.h for details on this method.
 */
void bb_port_set_data_direct32(BlackboardElement_T * element, const uint32_t data32, const uint8_t byteIndexInElement)
{
    // Get the offset/index into internal _dataWords given logical byte index within the element
    const uint32_t byteOffsetInWord = (uint32_t)(byteIndexInElement) % 4U;
    const uint32_t wordIndex = (uint32_t)(byteIndexInElement) / 4U;

    // Dereferencing a u32 pointer with a byte offset.  Make sure it is u32-aligned within the
    // element.
    SLI_ASSERT(byteOffsetInWord == 0);
    volatile uint32_t * const dest32Ptr = (volatile uint32_t *)(&element->dataWords[wordIndex]);

    *dest32Ptr = data32;
}

/**
 * Implementation of BlackboardPort interface that uses ARM load & store-exclusive instructions to
 * perform an atomic read from the internal blackboard element storage.
 *
 * @see BlackboardPort.h for details on this method.
 */
uint8_t bb_port_get_data_direct8(const BlackboardElement_T * element, const uint8_t byteIndexInElement)
{
    // Get the offset/index into internal _dataWords given logical byte index within the element
    const uint32_t byteOffsetInWord = (uint32_t)(byteIndexInElement) % 4U;
    const uint32_t wordIndex = (uint32_t)(byteIndexInElement) / 4U;

    volatile uint8_t * const src8Ptr = (volatile uint8_t *)(&element->dataWords[wordIndex]) + byteOffsetInWord;

    return *src8Ptr;
}

/**
 * Implementation of BlackboardPort interface that uses ARM load & store-exclusive instructions to
 * perform an atomic read from the internal blackboard element storage.
 *
 * @see BlackboardPort.h for details on this method.
 */
uint16_t bb_port_get_data_direct16(const BlackboardElement_T * element, const uint8_t byteIndexInElement)
{
    // Get the offset/index into internal _dataWords given logical byte index within the element
    const uint32_t byteOffsetInWord = (uint32_t)(byteIndexInElement) % 4U;
    const uint32_t wordIndex = (uint32_t)(byteIndexInElement) / 4U;

    // Dereferencing a u16 pointer with a byte offset.  Make sure it is u16-aligned within the
    // element.
    SLI_ASSERT((byteOffsetInWord % 2U) == 0);
    volatile uint16_t * const src16Ptr = (volatile uint16_t *)(((volatile uint8_t *)&element->dataWords[wordIndex]) + byteOffsetInWord);

    return *src16Ptr;
}

/**
 * Implementation of BlackboardPort interface that uses ARM load & store-exclusive instructions to
 * perform an atomic read from the internal blackboard element storage.
 *
 * @see BlackboardPort.h for details on this method.
 */
uint32_t bb_port_get_data_direct32(const BlackboardElement_T * element, const uint8_t byteIndexInElement)
{
    // Get the offset/index into internal _dataWords given logical byte index within the element
    const uint32_t byteOffsetInWord = (uint32_t)(byteIndexInElement) % 4U;
    const uint32_t wordIndex = (uint32_t)(byteIndexInElement) / 4U;

    // Dereferencing a u32 pointer with a byte offset.  Make sure it is u32-aligned within the
    // element.
    SLI_ASSERT(byteOffsetInWord == 0);
    volatile uint32_t * const src32Ptr = (volatile uint32_t *)(&element->dataWords[wordIndex]);

    return *src32Ptr;
}
