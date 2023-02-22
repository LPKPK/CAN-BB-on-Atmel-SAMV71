"""C++ Formatter Strings Class

Class containing strings to use for formatting C++ code

Copyright (c) 2021 Sunrise Labs, Inc.
All rights reserved.
"""


class CppFormatterSnippets:
    """Class containing strings to use for formatting C++ code"""
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
    GETTER_DECL_FMT = """   static {normalValueType} get_{baseName}();
"""

    # Format string for the setter declaration
    SETTER_DECL_FMT = """   static void set_{baseName}(const {normalValueType} value);

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
{normalValueType} BBoard::get_{baseName}()
{{
    return bb_get_{baseName}();
}}
"""

    # Format string for the setter definition
    SETTER_DEF_FMT = """
/**
 * @brief Sets the {baseName} value within the blackboard.
 * @see bb_get_{baseName} for a description of this property.
 * @param value {valueType} in {units}
 */
void BBoard::set_{baseName}(const {normalValueType} value)
{{
    bb_set_{baseName}(value);
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

#include "can_elem.h"


typedef enum
{{
{canSpecEnum}
    // Last item can be used as a count
    CAN_ELEM_LAST_MSG
}} CANElementIndex_T;


extern const CANElement_T can_spec_can_element_list[CAN_ELEM_LAST_MSG];

const CANElement_T * can_elem_get_can_elem_idx(const CANElementIndex_T idx);

const CANMsgInfo_T * can_elem_get_can_msg_info_idx(const CANElementIndex_T idx);
const CANMsgInfo_T * can_elem_get_can_msg_info_msgid(const uint32_t msgID);


// TODO: Make these checks in C-mode
//static_assert(!std::is_signed<std::underlying_type<Index>::type>::value, "Type is used as array index & should not be signed");
//static_assert((sizeof(list) / sizeof(list[0])) <= std::numeric_limits<std::underlying_type<Index>::type>::max(), "Overflow!");


"""

    CAN_SPEC_DEF_FMT = """
#include "can_spec.h"
#include "can_msg_info.h"

/// @brief Auto-generated {canSpecDesc} for the {canSpecName} (type = {canSpecType}) subsystem.

const CANElement_T can_spec_can_element_list[CAN_ELEM_LAST_MSG] =
{{
{canSpecTableRows}
}};

const CANElement_T * can_elem_get_can_elem_idx(const CANElementIndex_T idx)
{{
    return &can_spec_can_element_list[idx];
}}

const CANMsgInfo_T * can_elem_get_can_msg_info_idx(const CANElementIndex_T idx)
{{
    return &can_spec_can_element_list[idx].canMsgInfo;
}}

/**
 * @brief Returns the index of an element in the list of CANElements given a message ID
 * @param msgID The ID of the message
 *
 * @return The index of the element in the list with the given message ID
 * or CANElements::CAN_SPEC_LAST_MSG if it is not found
 */
const CANMsgInfo_T * can_elem_get_can_msg_info_msgid(const uint32_t msgID)
{{
    const CANMsgInfo_T * canMsgInfo = NULL;
    for (uint32_t i = 0U; i < (uint32_t)(CAN_ELEM_LAST_MSG); ++i)
    {{
        const CANElementIndex_T idx = (CANElementIndex_T)(i);
        if (can_spec_can_element_list[idx].canMsgInfo.id == msgID)
        {{
            canMsgInfo = &can_spec_can_element_list[idx].canMsgInfo;
            break;
        }}
    }}
    return canMsgInfo;
}}
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
