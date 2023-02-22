/** @file blackboard.h
*
* @brief Defines global elements & their accessors.
*
* @par
* Copyright (c) 2021 Sunrise Labs, Inc.
* All rights reserved.
*/

#ifndef BLACKBOARD_H
#define BLACKBOARD_H

#include "can_elem.h"
#include "bb_elem.h"
#include "bb_types.h"
#include "can_msg_info.h"

#if defined(__cplusplus)
extern "C"
{
#endif

typedef struct Blackboard_s
{
    BlackboardElement_T elements[CAN_ELEM_LAST_MSG];
} Blackboard_T;


void bb_init(void);

BlackboardElement_T * bb_get_element(const CANElementIndex_T idx);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// *** AUTOGENERATED BY BBGEN.  DO NOT EDIT THIS SECTION: BB_C_DECL

/// @brief Auto-generated Blackboard C declaration for the SCM (type = BB_C_DECL) subsystem.

int32_t bb_get_SCM_TestFloofy(void);
void bb_set_SCM_TestFloofy(const int32_t value);

float32_T bb_get_SCM_motor1Acceleration(void);
void bb_set_SCM_motor1Acceleration(const float32_T value);

float32_T bb_get_SCM_motor1Deceleration(void);
void bb_set_SCM_motor1Deceleration(const float32_T value);

float32_T bb_get_SCM_motor1MaxSpeed(void);
void bb_set_SCM_motor1MaxSpeed(const float32_T value);

float32_T bb_get_SCM_motor1MinSpeed(void);
void bb_set_SCM_motor1MinSpeed(const float32_T value);

int8_t bb_get_SCM_Motor1Control(void);
void bb_set_SCM_Motor1Control(const int8_t value);

int8_t bb_get_RCM_ModuleId(void);
void bb_set_RCM_ModuleId(const int8_t value);

float32_T bb_get_RCM_Sensor1Voltage(void);
void bb_set_RCM_Sensor1Voltage(const float32_T value);

float32_T bb_get_RCM_Sensor1Temperature(void);
void bb_set_RCM_Sensor1Temperature(const float32_T value);

float32_T bb_get_RCM_Sensor1Foo(void);
void bb_set_RCM_Sensor1Foo(const float32_T value);

float32_T bb_get_RCM_Sensor1Bar(void);
void bb_set_RCM_Sensor1Bar(const float32_T value);


// *** AUTOGENERATED BY BBGEN.  DO NOT EDIT THIS SECTION: BB_C_DECL
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#if defined(__cplusplus)
}
#endif

#endif // BLACKBOARD_H
