/** @file scm_default_callbacks.c
*
* @brief Definitions of default auto generated callback functions for the
* blackboard messages. Weakly linked to allow overriding.
*
* @par
* Copyright (c) 2021 Sunrise Labs, Inc.
* All rights reserved.
*/

#include "scm_callbacks.h"

#ifdef USING_GCC
#define MAKE_WEAK(f) __attribute__((weak)) f
#endif // USING_GCC

#ifdef USING_MPLAB
#define MAKE_WEAK(f) __attribute__((weak)) f
#endif // USING_MPLAB

#ifdef USING_IAR
#define MAKE_WEAK(f) __weak f
#endif // USING_IAR


#include <stdio.h>
#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// *** AUTOGENERATED BY BBGEN.  DO NOT EDIT THIS SECTION: BB_C_DEFAULT_CB

/// @brief Auto-generated default weakly linked callbacks for the SCM subsystem.


/**
 * @brief testSCMCallback - callback for message SystemBarfy
 * @param param Pointer to user defined context
 * @param rxChan If the callback is occurring on receipt of this variable, this is
 *              the channel the message was received on. Otherwise, CAN_NONE.
 * @param txChan If the callback is occurring on transmission of this variable, this
 *              is the channel the message was transmitted on. Otherwise, CAN_NONE.
 */
MAKE_WEAK(void testSCMCallback(void * context, CanChannel_T rxChan, CanChannel_T txChan))
{
    (void) context; // Unused - stub
    (void) rxChan;  // Unused - stub
    (void) txChan;  // Unused - stub
}

// *** AUTOGENERATED BY BBGEN.  DO NOT EDIT THIS SECTION: BB_C_DEFAULT_CB
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
