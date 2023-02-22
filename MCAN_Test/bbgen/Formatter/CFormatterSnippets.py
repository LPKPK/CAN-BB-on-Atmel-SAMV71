"""C Formatter Strings Class

Class containing strings to use for formatting C code

Copyright (c) 2021 Sunrise Labs, Inc.
All rights reserved.
"""


class CFormatterSnippets:
    """Class containing strings to use for formatting C code"""
    MESSAGE_FMT = """            AddEntry(new CanMessageEntry()
            {{
                Name = "{name}", DataLength = {dataLength}, Identifier = 0x{msgId:X},
                Description = @"{description}"
            }}
            """

    VARIABLE_FMT = """            .AddVariable(new CanMessageVariable()
            {{
                Name = "{name}", Units = "{units}", ResolutionBits = {resolutionBits},
                IsSigned = {isSigned}, Offset = {offset}, Length = {length},
                Description = @"{description}"
            }})"""

    MESSAGE_ENUM_LINES_FMT = """
        public enum CanMessageIndex
        {{
{enumIndexLines}        }}

        public enum CanMessageID
        {{
{enumLines}        }}

"""

    MESSAGE_ENUM_INDEX_LINE_FMT = """            {nameStr:<40} {index},
"""

    MESSAGE_ENUM_LINE_FMT = """            {nameStr:<40} {msgId},
"""

    # Format string for the getter declaration
    GETTER_DECL_FMT = """{normalValueType} bb_get_{baseName}(void);
"""

    # Format string for the setter declaration
    SETTER_DECL_FMT = """void bb_set_{baseName}(const {normalValueType} value);

"""

    BB_DECL_FMT = """
/// @brief Auto-generated {canSpecDesc} for the {canSpecName} (type = {canSpecType}) subsystem.

{bbDecls}
"""

    GETTER_DEF_FMT = """
/**
 * @brief Gets the {baseName} value from the blackboard.
 * {description}
 * @return {valueType} value of {baseName} in {units}
 */
{normalValueType} bb_get_{baseName}(void)
{{
    const CANMsgInfo_T * canMsgInfo = can_elem_get_can_msg_info_idx(CAN_ELEM_{msgFullName});
    const BlackboardElement_T * const bbElem = bb_get_element(CAN_ELEM_{msgFullName});
    const CANDataInfo_T * dataInfo = &canMsgInfo->data_info[{msgSubIndex}];
    const {valueType} value = bb_elem_get_{selector}_data(bbElem, dataInfo->start_byte);
    {returnStatement}
}}
"""

    # Format string for the setter definition
    SETTER_DEF_FMT = """
/**
 * @brief Sets the {baseName} value within the blackboard.
 * @see bb_get_{baseName} for a description of this property.
 * @param value {valueType} in {units}
 */
void bb_set_{baseName}(const {normalValueType} value)
{{
    const CANMsgInfo_T * canMsgInfo = can_elem_get_can_msg_info_idx(CAN_ELEM_{msgFullName});
    BlackboardElement_T * bbElem = bb_get_element(CAN_ELEM_{msgFullName});
    const CANDataInfo_T * dataInfo = &canMsgInfo->data_info[{msgSubIndex}];
    bb_elem_set_{selector}_data(bbElem, {convertDataExpression}, dataInfo->start_byte);
}}
"""

    BB_DEF_FMT = """

/// @brief Auto-generated {canSpecDesc} for the {canSpecName} (type = {canSpecType}) subsystem.

{bbDefs}

"""

    CAN_SPEC_ENUM_ROW_FMT = """    CAN_ELEM_{enumName},
"""

    # Format string for a CAN specialization class
    CAN_SPEC_DECL_FMT = """

/// @brief Auto-generated {canSpecDesc} for the {canSpecName} (type = {canSpecType}) subsystem.

typedef enum
{{
{canSpecEnum}
    // Last item can be used as a count
    CAN_ELEM_LAST_MSG
}} CANElementIndex_T;

// can_elem.h and can_spec.h have a carefully crafted inter-dependency since they are really one
// file auto-generated in one half and fixed code in the other. Take care about changing the
// order/location of includes.
#include "can_elem.h"
extern const CANElement_T can_spec_can_element_list[CAN_ELEM_LAST_MSG];

"""

    CAN_SPEC_DEF_FMT = """
/// @brief Auto-generated {canSpecDesc} for the {canSpecName} (type = {canSpecType}) subsystem.

const CANElement_T can_spec_can_element_list[CAN_ELEM_LAST_MSG] =
{{
{canSpecTableRows}
}};

"""

    CAN_SPEC_DEF_VAR_MAP_ROW_FMT = """                {{ {variableName:<34} {signSpec:<18} {startByte}         {byteCount} }},
"""

    CAN_SPEC_DEF_ROW_FMT = """    {{
        /* .canMsgInfo */
        {{
            0x{msgId:03X}                                    /* id        */,
            {messageName:<40} /* name      */,
            {periodMs:<5}                                    /* period_ms */,
            {dataLength}                                        /* numBytes  */,
            {{   /* data_info */
                /* name                              signed_val         start_byte byte_count */
{varMapRows}
            }}
        }},
        {{
            {callback:<40} /* pCallback */,
            {callbackContext:<40} /* callbackParam */
        }},
        {txChannels}  /* txChan */,
        {rxSections}  /* rxChan */
    }},
"""

    CAN_SPEC_SCHED_DEF_FMT = """
/// @brief Auto-generated {canSpecDesc} for the {canSpecName} (type = {canSpecType}) subsystem.

static constexpr CANElement_T can_spec_can_element_sched_list[CAN_ELEM_LAST_MSG] =
{{
{canSpecTableRows}
}};

"""

    CAN_SPEC_SCHED_DEF_VAR_MAP_ROW_FMT = """                {{ {variableName:<34} {signSpec:<18} {startByte}         {byteCount} }},
"""

    CAN_SPEC_SCHED_DEF_ROW_FMT = """    {{
        /* .canMsgInfo */
        {{
            0x{msgId:03X}                                    /* id        */,
            {messageName:<40} /* name      */,
            {periodMs:<5}                                    /* period_ms */,
            {dataLength}                                        /* numBytes  */,
            {{   /* data_info */
                /* name                              signed_val         start_byte byte_count */
{varMapRows}
            }}
        }},
        {{
            /* Callbacks always null for scheduling calculations */
            NULL,
            NULL
        }},
        {txChannels}  /* txChan */,
        {rxSections}  /* rxChan */
    }},
"""

    BB_DEFAULT_CB_FMT = """
/// @brief Auto-generated default weakly linked callbacks for the {canSpecName} subsystem.

{callbacks}
"""

    BB_DEFAULT_CALLBACK = """
/**
 * @brief {callbackName} - callback for message {messageName}
 * @param param Pointer to user defined context
 * @param rxChan If the callback is occurring on receipt of this variable, this is
 *              the channel the message was received on. Otherwise, CAN_NONE.
 * @param txChan If the callback is occurring on transmission of this variable, this
 *              is the channel the message was transmitted on. Otherwise, CAN_NONE.
 */
MAKE_WEAK(void {callbackName}(void * context, CanChannel_T rxChan, CanChannel_T txChan))
{{
    (void) context; // Unused - stub
    (void) rxChan;  // Unused - stub
    (void) txChan;  // Unused - stub
}}
"""

    BB_DEFAULT_CB_DECL_FMT = """
/// @brief Auto-generated declarations for callbacks for the {canSpecName} subsystem.

{callbackDecls}
"""

    BB_CALLBACK_DECL_FMT = """
void {callbackName}(void * context, CanChannel_T rxChan, CanChannel_T txChan);
"""
