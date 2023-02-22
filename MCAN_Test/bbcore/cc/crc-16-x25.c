/**
 * @file crc-16-x25.c
 *
 * Copyright (C) 2017 Sunrise Labs, Inc.
 * All rights reserved
 *
 * @brief This file implements the CRC-16-X25 algorithm using a lookup table.
 *
 *
 */

#include "crc-16-x25.h"
#include <stddef.h>

/* Rocksoft parameters...
   Name   : X-25
   Width  : 16
   Poly   : 0x1021
   Init   : 0xFFFF
   RefIn  : TRUE
   RefOut : TRUE
   XorOut : 0xffffL
   Check  : 0x906e
*/

/**
 * @brief  Calculates the CRC for the buffer of bytes.
 *         Uses a predefined table to calculate the CRC-16-X.25
 *         of the buffer passed in over the length specified.
 * @param  p_buf: pointer to a byte (uint8_t) buffer
 * @param  count: count of bytes in the buffer over which to calculate the CRC
 *
 * @retval crc16 value of the buffer
 */
//lint -e{765} external symbol 'calc_crc_16_x25' could be made static [MISRA 2012 Rule 8.7, advisory]
uint16_t calc_crc_16_x25(uint8_t const *p_buf, uint16_t count)
{
    uint16_t crc_16_x25_table[256] =
    {
        0x0000u, 0x1189u, 0x2312u, 0x329Bu, 0x4624u, 0x57ADu, 0x6536u, 0x74BFu,
        0x8C48u, 0x9DC1u, 0xAF5Au, 0xBED3u, 0xCA6Cu, 0xDBE5u, 0xE97Eu, 0xF8F7u,
        0x1081u, 0x0108u, 0x3393u, 0x221Au, 0x56A5u, 0x472Cu, 0x75B7u, 0x643Eu,
        0x9CC9u, 0x8D40u, 0xBFDBu, 0xAE52u, 0xDAEDu, 0xCB64u, 0xF9FFu, 0xE876u,
        0x2102u, 0x308Bu, 0x0210u, 0x1399u, 0x6726u, 0x76AFu, 0x4434u, 0x55BDu,
        0xAD4Au, 0xBCC3u, 0x8E58u, 0x9FD1u, 0xEB6Eu, 0xFAE7u, 0xC87Cu, 0xD9F5u,
        0x3183u, 0x200Au, 0x1291u, 0x0318u, 0x77A7u, 0x662Eu, 0x54B5u, 0x453Cu,
        0xBDCBu, 0xAC42u, 0x9ED9u, 0x8F50u, 0xFBEFu, 0xEA66u, 0xD8FDu, 0xC974u,
        0x4204u, 0x538Du, 0x6116u, 0x709Fu, 0x0420u, 0x15A9u, 0x2732u, 0x36BBu,
        0xCE4Cu, 0xDFC5u, 0xED5Eu, 0xFCD7u, 0x8868u, 0x99E1u, 0xAB7Au, 0xBAF3u,
        0x5285u, 0x430Cu, 0x7197u, 0x601Eu, 0x14A1u, 0x0528u, 0x37B3u, 0x263Au,
        0xDECDu, 0xCF44u, 0xFDDFu, 0xEC56u, 0x98E9u, 0x8960u, 0xBBFBu, 0xAA72u,
        0x6306u, 0x728Fu, 0x4014u, 0x519Du, 0x2522u, 0x34ABu, 0x0630u, 0x17B9u,
        0xEF4Eu, 0xFEC7u, 0xCC5Cu, 0xDDD5u, 0xA96Au, 0xB8E3u, 0x8A78u, 0x9BF1u,
        0x7387u, 0x620Eu, 0x5095u, 0x411Cu, 0x35A3u, 0x242Au, 0x16B1u, 0x0738u,
        0xFFCFu, 0xEE46u, 0xDCDDu, 0xCD54u, 0xB9EBu, 0xA862u, 0x9AF9u, 0x8B70u,
        0x8408u, 0x9581u, 0xA71Au, 0xB693u, 0xC22Cu, 0xD3A5u, 0xE13Eu, 0xF0B7u,
        0x0840u, 0x19C9u, 0x2B52u, 0x3ADBu, 0x4E64u, 0x5FEDu, 0x6D76u, 0x7CFFu,
        0x9489u, 0x8500u, 0xB79Bu, 0xA612u, 0xD2ADu, 0xC324u, 0xF1BFu, 0xE036u,
        0x18C1u, 0x0948u, 0x3BD3u, 0x2A5Au, 0x5EE5u, 0x4F6Cu, 0x7DF7u, 0x6C7Eu,
        0xA50Au, 0xB483u, 0x8618u, 0x9791u, 0xE32Eu, 0xF2A7u, 0xC03Cu, 0xD1B5u,
        0x2942u, 0x38CBu, 0x0A50u, 0x1BD9u, 0x6F66u, 0x7EEFu, 0x4C74u, 0x5DFDu,
        0xB58Bu, 0xA402u, 0x9699u, 0x8710u, 0xF3AFu, 0xE226u, 0xD0BDu, 0xC134u,
        0x39C3u, 0x284Au, 0x1AD1u, 0x0B58u, 0x7FE7u, 0x6E6Eu, 0x5CF5u, 0x4D7Cu,
        0xC60Cu, 0xD785u, 0xE51Eu, 0xF497u, 0x8028u, 0x91A1u, 0xA33Au, 0xB2B3u,
        0x4A44u, 0x5BCDu, 0x6956u, 0x78DFu, 0x0C60u, 0x1DE9u, 0x2F72u, 0x3EFBu,
        0xD68Du, 0xC704u, 0xF59Fu, 0xE416u, 0x90A9u, 0x8120u, 0xB3BBu, 0xA232u,
        0x5AC5u, 0x4B4Cu, 0x79D7u, 0x685Eu, 0x1CE1u, 0x0D68u, 0x3FF3u, 0x2E7Au,
        0xE70Eu, 0xF687u, 0xC41Cu, 0xD595u, 0xA12Au, 0xB0A3u, 0x8238u, 0x93B1u,
        0x6B46u, 0x7ACFu, 0x4854u, 0x59DDu, 0x2D62u, 0x3CEBu, 0x0E70u, 0x1FF9u,
        0xF78Fu, 0xE606u, 0xD49Du, 0xC514u, 0xB1ABu, 0xA022u, 0x92B9u, 0x8330u,
        0x7BC7u, 0x6A4Eu, 0x58D5u, 0x495Cu, 0x3DE3u, 0x2C6Au, 0x1EF1u, 0x0F78u
};

    uint16_t crc = 0xffffu;

    if (p_buf != NULL)
    {
        while (0u < count)
        {
            crc    = crc_16_x25_table[(crc ^ *p_buf) & 0xffu] ^ ((crc >> 8) & 0xffu);
            // 9044 function parameter modified [MISRA 2012 Rule 17.8, advisory]
            // 9016 performing pointer arithmetic via addition [MISRA 2012 Rule 18.4, advisory]
            count -= 1u;        //lint !e9044
            p_buf += 1u;        //lint !e9044 !e9016
        }
    }
    return (crc ^ 0xffffu);
}

#ifdef CRC_16_X25_UNIT_TEST

// Build the unit test using command line:
// cc -o crc-16-x25 -DCRC_16_X25_UNIT_TEST crc-16-x25.c

#include <stdio.h>

int main (int argc, char **argv)
{
    /* from Appendix I of X.25 standard */
    static const uint8_t m0[4] = { 0x03, 0x3f, 0x5b, 0xec }; // the CRC is last two bytes in little endian order
    static const uint8_t m1[4] = { 0x01, 0x73, 0x83, 0x57 };
    static const uint8_t m2[4] = { 0x01, 0x3f, 0xeb, 0xdf };
    static const uint8_t m3[4] = { 0x03, 0x73, 0x33, 0x64 };

    uint16_t t0 = calc_crc_16_x25(m0, 2);
    uint16_t t1 = calc_crc_16_x25(m1, 2);
    uint16_t t2 = calc_crc_16_x25(m2, 2);
    uint16_t t3 = calc_crc_16_x25(m3, 2);

    // ITU-T Recommendation X.25 (October 1996)
    // https://www.itu.int/rec/T-REC-X.25/en
    // I Definition: Residue; full mathematical description (Section 2.2.7.4, p.9)
    // IV 4 codewords (Appendix I, p.145)
    //   033F5BEC
    //   01738357
    //   013FEBDF
    //   03733364

    // X.25 standard says CRC should yield 0001_1101_0000_1111
    // Rocksoft residue 0xf0b8 (reversed)
    // inverted is 0x0f47

    if ((t0 == ((m0[3] << 8) | m0[2])) &&
        (t1 == ((m1[3] << 8) | m1[2])) &&
        (t2 == ((m2[3] << 8) | m2[2])) &&
        (t3 == ((m3[3] << 8) | m3[2])) &&
        (0x0f47 == calc_crc_16_x25(m0, 4)) &&
        (0x0f47 == calc_crc_16_x25(m1, 4)) &&
        (0x0f47 == calc_crc_16_x25(m2, 4)) &&
        (0x0f47 == calc_crc_16_x25(m3, 4)))
    {
        static const char *tststr = "123456789";
        printf("Pass 0x%04x\n", calc_crc_16_x25((uint8_t const * )tststr, 9));
    }
    else
    {
        printf("X.25 m0 is 0x%02x%02x expected 0x%02x%02x\n", t0 & 0xffu, t0 >> 8, m0[2], m0[3]);
        printf("X.25 m1 is 0x%02x%02x expected 0x%02x%02x\n", t1 & 0xffu, t1 >> 8, m1[2], m1[3]);
        printf("X.25 m2 is 0x%02x%02x expected 0x%02x%02x\n", t2 & 0xffu, t2 >> 8, m2[2], m2[3]);
        printf("X.25 m3 is 0x%02x%02x expected 0x%02x%02x\n", t3 & 0xffu, t3 >> 8, m3[2], m3[3]);
        printf("X.25 m0 is 0x%04x expected 0x0f47\n", calc_crc_16_x25(m0, 4));
        printf("X.25 m1 is 0x%04x expected 0x0f47\n", calc_crc_16_x25(m1, 4));
        printf("X.25 m2 is 0x%04x expected 0x0f47\n", calc_crc_16_x25(m2, 4));
        printf("X.25 m3 is 0x%04x expected 0x0f47\n", calc_crc_16_x25(m3, 4));
    }
}

#endif
