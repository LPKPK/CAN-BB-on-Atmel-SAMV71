/** @file canspec.c
*
* @brief Defines a list of can elements for the SCM
*
* @par
* Copyright (c) 2021 Sunrise Labs, Inc.
* All rights reserved.
*/

#include "can_spec.h"
#include "can_msg_info.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// *** AUTOGENERATED BY BBGEN.  DO NOT EDIT THIS SECTION: CAN_SPEC_C_DEF

/// @brief Auto-generated CAN specialization definition for the SCM (type = CAN_SPEC_C_DEF) subsystem.

const CANElement_T can_spec_can_element_list[CAN_ELEM_LAST_MSG] =
{
    {
        /* .canMsgInfo */
        {
            0x400                                    /* id        */,
            "SCM_SystemBarfy"                        /* name      */,
            100                                      /* period_ms */,
            4                                        /* numBytes  */,
            {   /* data_info */
                /* name                              signed_val         start_byte byte_count */
                { "TestFloofy",                      CAN_DATA_SIGNED,   0,         4 },
                { NULL_STR,                          CAN_DATA_UNSIGNED, 0,         0 },
                { NULL_STR,                          CAN_DATA_UNSIGNED, 0,         0 },
                { NULL_STR,                          CAN_DATA_UNSIGNED, 0,         0 },

            }
        },
        {
            &testSCMCallback                         /* pCallback */,
            (void *)(SCM_EVENT_FOO)                  /* callbackParam */
        },
        (CanChannel_T)((uint16_t)(CAN_CHAN1) | (uint16_t)(CAN_CHAN2))  /* txChan */,
        CAN_CHAN1  /* rxChan */
    },
    {
        /* .canMsgInfo */
        {
            0x410                                    /* id        */,
            "SCM_Motor1Params1"                      /* name      */,
            100                                      /* period_ms */,
            8                                        /* numBytes  */,
            {   /* data_info */
                /* name                              signed_val         start_byte byte_count */
                { "motor1Acceleration",              CAN_DATA_SIGNED,   0,         2 },
                { "motor1Deceleration",              CAN_DATA_SIGNED,   2,         2 },
                { "motor1MaxSpeed",                  CAN_DATA_SIGNED,   4,         2 },
                { "motor1MinSpeed",                  CAN_DATA_SIGNED,   6,         2 },

            }
        },
        {
            NULL                                     /* pCallback */,
            NULL                                     /* callbackParam */
        },
        CAN_CHAN1  /* txChan */,
        CAN_NONE  /* rxChan */
    },
    {
        /* .canMsgInfo */
        {
            0x460                                    /* id        */,
            "SCM_Motor1Control"                      /* name      */,
            100                                      /* period_ms */,
            1                                        /* numBytes  */,
            {   /* data_info */
                /* name                              signed_val         start_byte byte_count */
                { "Motor1Control",                   CAN_DATA_SIGNED,   0,         1 },
                { NULL_STR,                          CAN_DATA_UNSIGNED, 0,         0 },
                { NULL_STR,                          CAN_DATA_UNSIGNED, 0,         0 },
                { NULL_STR,                          CAN_DATA_UNSIGNED, 0,         0 },

            }
        },
        {
            NULL                                     /* pCallback */,
            NULL                                     /* callbackParam */
        },
        CAN_CHAN1  /* txChan */,
        CAN_NONE  /* rxChan */
    },
    {
        /* .canMsgInfo */
        {
            0x470                                    /* id        */,
            "SCM_Motor1Contro2"                      /* name      */,
            100                                      /* period_ms */,
            1                                        /* numBytes  */,
            {   /* data_info */
                /* name                              signed_val         start_byte byte_count */
                { "Motor2Control",                   CAN_DATA_SIGNED,   0,         1 },
                { NULL_STR,                          CAN_DATA_UNSIGNED, 0,         0 },
                { NULL_STR,                          CAN_DATA_UNSIGNED, 0,         0 },
                { NULL_STR,                          CAN_DATA_UNSIGNED, 0,         0 },

            }
        },
        {
            NULL                                     /* pCallback */,
            NULL                                     /* callbackParam */
        },
        CAN_CHAN1  /* txChan */,
        CAN_NONE  /* rxChan */
    },
    {
        /* .canMsgInfo */
        {
            0x480                                    /* id        */,
            "SCM_Motor3Control"                      /* name      */,
            100                                      /* period_ms */,
            1                                        /* numBytes  */,
            {   /* data_info */
                /* name                              signed_val         start_byte byte_count */
                { "Motor3Control",                   CAN_DATA_SIGNED,   0,         1 },
                { NULL_STR,                          CAN_DATA_UNSIGNED, 0,         0 },
                { NULL_STR,                          CAN_DATA_UNSIGNED, 0,         0 },
                { NULL_STR,                          CAN_DATA_UNSIGNED, 0,         0 },

            }
        },
        {
            NULL                                     /* pCallback */,
            NULL                                     /* callbackParam */
        },
        CAN_CHAN1  /* txChan */,
        CAN_NONE  /* rxChan */
    },
    {
        /* .canMsgInfo */
        {
            0x630                                    /* id        */,
            "RCM_System"                             /* name      */,
            100                                      /* period_ms */,
            1                                        /* numBytes  */,
            {   /* data_info */
                /* name                              signed_val         start_byte byte_count */
                { "ModuleId",                        CAN_DATA_SIGNED,   0,         1 },
                { NULL_STR,                          CAN_DATA_UNSIGNED, 0,         0 },
                { NULL_STR,                          CAN_DATA_UNSIGNED, 0,         0 },
                { NULL_STR,                          CAN_DATA_UNSIGNED, 0,         0 },

            }
        },
        {
            &SCMCallback_print                       /* pCallback */,
            (void *)(SCM_EVENT)                      /* callbackParam */
        },
        CAN_NONE  /* txChan */,
        CAN_CHAN1  /* rxChan */
    },
    {
        /* .canMsgInfo */
        {
            0x631                                    /* id        */,
            "RCM_Sensor1"                            /* name      */,
            100                                      /* period_ms */,
            8                                        /* numBytes  */,
            {   /* data_info */
                /* name                              signed_val         start_byte byte_count */
                { "Sensor1Voltage",                  CAN_DATA_SIGNED,   0,         2 },
                { "Sensor1Temperature",              CAN_DATA_SIGNED,   2,         2 },
                { "Sensor1Foo",                      CAN_DATA_SIGNED,   4,         2 },
                { "Sensor1Bar",                      CAN_DATA_SIGNED,   6,         2 },

            }
        },
        {
            NULL                                     /* pCallback */,
            NULL                                     /* callbackParam */
        },
        CAN_NONE  /* txChan */,
        CAN_CHAN1  /* rxChan */
    },

};

// *** AUTOGENERATED BY BBGEN.  DO NOT EDIT THIS SECTION: CAN_SPEC_C_DEF
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
