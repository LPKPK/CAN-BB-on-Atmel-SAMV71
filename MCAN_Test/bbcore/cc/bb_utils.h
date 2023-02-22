/** @file bb_utils.h
*
* @brief Contains utility functions, map message ID to CAN Index
*
* @par
* Copyright (c) 2021 Sunrise Labs, Inc.
* All rights reserved.
*/
#ifndef BLACKBOARDUTILS_H
#define BLACKBOARDUTILS_H

#include <stdint.h>
#include "bb_port.h"

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * Converts the given value into its signed two's complement on any platform.
 *
 * This works by toggling the most significant bit and adding half the range of the data type
 * size.  It has been demonstrated on GCC this is completely optimized out and replaced with a
 * simple cast when full-speed optimization is enabled.
 *
 * @return a two's complement int8_t from the given uint8_t
 */
static inline int8_t bb_utils_toSigned2sComp8(const uint8_t value)
{
    // Note: Casts are crafted to avoid warnings about implicit conversions in operators
    return (int8_t)((int16_t)((uint8_t)(value ^ 0x80U)) + (int16_t)(-128));
}

/**
 * Converts the given value into its signed two's complement on any platform.
 *
 * This works by toggling the most sigificant bit and adding half the range of the date type
 * size.  It has been demonstrated on GCC this is completely optimized out and replaced with a
 * simple cast when full-speed optimization is enabled.
 *
 * @return a two's complement int16_t from the given uint16_t
 */
static inline int16_t bb_utils_toSigned2sComp16(const uint16_t value)
{
    return (int16_t)(((int32_t)((uint16_t)(value ^ 0x8000U)) + (int32_t)(-32768)));
}

/**
 * Converts the given value into its signed two's complement on any platform.
 *
 * This works by toggling the most sigificant bit and adding half the range of the date type
 * size.  It has been demonstrated on GCC this is completely optimized out and replaced with a
 * simple cast when full-speed optimization is enabled.
 *
 * @return a two's complement int32_t from the given uint32_t
 */
static inline int32_t bb_utils_toSigned2sComp32(const uint32_t value)
{
    return (int32_t)(((int64_t)((uint32_t)(value ^ 0x80000000U)) + (int64_t)(-2147483648)));
}

/**
 * Converts the given value into its unsigned version on any platform.
 *
 * This works by simply casting which is a defined C behavior.
 *
 * @return a uint8_t from the given int8_t
 */
static inline uint8_t bb_utils_toUnsigned8(const int8_t value)
{
    return (uint8_t)value;
}

/**
 * Converts the given value into its unsigned version on any platform.
 *
 * This works by simply casting which is a defined C behavior.
 *
 * @return a uint16_t from the given int16_t
 */
static inline uint16_t bb_utils_toUnsigned16(const int16_t value)
{
    return (uint16_t)value;
}

/**
 * Converts the given value into its unsigned version on any platform.
 *
 * This works by simply casting which is a defined C behavior.
 *
 * @return a uint32_t from the given int32_t
 */
static inline uint32_t bb_utils_toUnsigned32(const int32_t value)
{
    return (uint32_t)value;
}

/**
 * @brief Converts fixed point with given resolution to floating point
 *
 * @param ivalue Fixed point value
 *
 * @param bits Resolution bits
 *
 * @return Converted floating point value
 */
static inline float32_T bb_utils_fixed16ToFloat(const int16_t ivalue, const uint16_t bits)
{
    const uint32_t denominator = 1UL << bits;
    const float32_T fdenominator = (float32_T)(denominator);
    const float32_T fnumerator = (float32_T)(ivalue);
    return fnumerator / fdenominator;
}

/**
 * @brief Converts floating point to fixed point with given resolution
 *
 * @param fvalue Floating point value
 *
 * @param bits Resolution bits
 *
 * @return Converted fixed point value
 */
static inline int16_t bb_utils_floatToFixed16(const float32_T fvalue, const uint16_t bits)
{
    const uint32_t shift = 1UL << bits;
    const float32_T fshift = (float32_T)(shift);
    const float32_T fproduct = fvalue * fshift;
    return (int16_t)(fproduct);
}

/**
 * @brief Converts fixed point with given resolution to floating point
 *
 * @param ivalue Fixed point value
 *
 * @param bits Resolution bits
 *
 * @return Converted floating point value
 */
static inline float32_T bb_utils_fixed32ToFloat(const int32_t ivalue, const uint16_t bits)
{
    const uint32_t denominator = 1UL << bits;
    const float32_T fdenominator = (float32_T)(denominator);
    const float32_T fnumerator = (float32_T)(ivalue);
    return fnumerator / fdenominator;
}

/**
 * @brief Converts fixed point with given resolution to double (64-bit)
 *
 * @param ivalue Fixed point value
 *
 * @param bits Resolution bits
 *
 * @return Converted double value
 */
static inline float64_T bb_utils_fixed16ToFloat64(const int16_t ivalue, const uint16_t bits)
{
    const uint32_t denominator = 1UL << bits;
    const float64_T fdenominator = (float64_T)(denominator);
    const float64_T fnumerator = (float64_T)(ivalue);
    return fnumerator / fdenominator;
}

/**
 * @brief Converts double to fixed point with given resolution
 *
 * @param fvalue Double value
 *
 * @param bits Resolution bits
 *
 * @return Converted fixed point value
 */
static inline int16_t bb_utils_float64ToFixed16(const float64_T fvalue, const uint16_t bits)
{
    const uint32_t shift = 1UL << bits;
    const float64_T fshift = (float64_T)(shift);
    const float64_T fproduct = fvalue * fshift;
    return (int16_t)(fproduct);
}

/*
 * @brief Returns the current tick count
 *
 * @return The current tick count
 */
static inline uint32_t bb_utils_get_current_tick(void)
{
    return bb_port_get_current_ticks();
}

#if defined(__cplusplus)
}
#endif

#endif //BLACKBOARDUTILS_H
