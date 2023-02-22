/**
 * @file crc-16-x25.h
 *
 * Copyright (C) 2017 Sunrise Labs, Inc.
 * All rights reserved
 *
 * @brief This file implements the CRC-16-X25 algorithm using a lookup table.
 *
 *
 */

#ifndef SLI_CRC16_X25_H
#define SLI_CRC16_X25_H

#include <stdint.h>
#ifdef __cplusplus
extern "C"
{
#endif

// Cannot make this function static (MPU) or move it (needs to be included in other files)
//lint -e{765} external symbol 'calc_crc_16_x25' could be made static [MISRA 2012 Rule 8.7, advisory]
//lint -e{759} header declaration for symbol 'calc_crc_16_x25(const uint8_t *, uint16_t)' could be moved from header to module
uint16_t calc_crc_16_x25 (uint8_t const *p_buf, uint16_t count);

#ifdef __cplusplus
}
#endif


#endif // SLI_CRC16_X25_H
