/** @file can_elem.c
*
* @brief Basic CAN data structure.
*
* @par
* Copyright (c) 2021 Sunrise Labs, Inc.
* All rights reserved.
*/
#include "can_elem.h"
#include "sli_assert.h"


#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * @brief Returns a pointer to the can element struct associated with the given index
 *
 * @param idx Enum representing the index of the element to return
 *
 * @return A pointer to the can element struct with the given index
 */
const CANElement_T * can_elem_get_can_elem_idx(const CANElementIndex_T idx)
{
    return &can_spec_can_element_list[idx];
}

/**
 * @brief Returns a pointer to the can msg info struct associated with the given index
 *
 * @param idx Enum representing the index of the element to return
 *
 * @return A pointer to the can msg info struct with the given index
 */
const CANMsgInfo_T * can_elem_get_can_msg_info_idx(const CANElementIndex_T idx)
{
    return &can_spec_can_element_list[idx].canMsgInfo;
}

/**
 * @brief Returns the index of an element in the list of CANElements given a message ID
 * @param msgID The ID of the message
 * @param[out] foundIdx A pointer to the CANElementIndex_T index of the corresponding msgID; this can be used
 * to lookup the same object in the BlackboardElements list; if the message was not found, this 
 * variable is not modified; Must be non-NULL
 *
 * @return The index of the element in the list with the given message ID
 * or CANElements::CAN_SPEC_LAST_MSG if it is not found
 */
const CANMsgInfo_T * can_elem_get_can_msg_info_msgid(const uint32_t msgID, CANElementIndex_T *foundIdx)
{
    SLI_ASSERT(foundIdx != NULL);
    const CANMsgInfo_T * canMsgInfo = NULL;
    for (uint32_t i = 0U; i < (uint32_t)(CAN_ELEM_LAST_MSG); ++i)
    {
        const CANElementIndex_T idx = (CANElementIndex_T)(i);
        if (can_spec_can_element_list[idx].canMsgInfo.id == msgID)
        {
            canMsgInfo = &can_spec_can_element_list[idx].canMsgInfo;
            *foundIdx = idx;
            break;
        }
    }
    return canMsgInfo;
}


#if defined(__cplusplus)
}
#endif
