/** @file scm_callbacks.c
*
* @brief User defined callback functions for the blackboard messages
*
* @par
* Copyright (c) 2021 Sunrise Labs, Inc.
* All rights reserved.
*/

#include "can_elem.h"
#include "scm_callbacks.h"

#include <stdio.h>

static int hackyStickyCounter = 0;

void testSCMCallback(void * SCM_EVENT_FOO, CanChannel_T rxChan, CanChannel_T txChan)
{
    SCMEvent_T scmEvent = (SCMEvent_T)(uint32_t)(uint64_t)(SCM_EVENT_FOO);
    // printf("testCallback: SCM_EVENT_FOO=%d, rxChan=%d, txChan=%d, counter=%d\n", scmEvent, rxChan, txChan, hackyStickyCounter++);
}


void SCMCallback_print(void * SCM_EVENT, CanChannel_T rxChan, CanChannel_T txChan){
    printf("Receive =%d\n", hackyStickyCounter++);
}