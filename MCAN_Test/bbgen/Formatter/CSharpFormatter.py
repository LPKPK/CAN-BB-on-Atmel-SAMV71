"""C# Formatter Strings Class

Class containing strings to use for formatting C# code

Copyright (c) 2021 Sunrise Labs, Inc.
All rights reserved.
"""

#TODO: Refactor this formatter to use the new Base Formatter scheme

class CSharpFormatter:
    """Class containing strings to use for formatting C# code"""

    def __init__(self, blackboard):
        self.blackboard = blackboard

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

    def get_vars_list(self):
        msg_lines = ''

        for message in self.blackboard.messages:
            msg_line = self.MESSAGE_FMT.format(
                name=message.get_full_name(),
                description=message.description,
                msgId=message.id,
                dataLength=message.get_size_bytes())

            var_lines = ''
            for variable in message.variables:
                var_line = self.VARIABLE_FMT.format(
                    name=variable.get_full_name(),
                    units=variable.units,
                    resolutionBits=variable.resolution_bits,
                    isSigned="true" if variable.is_signed else "false",
                    offset=variable.get_offset_bytes(),
                    length=variable.get_size_bytes(),
                    description=variable.description)
                var_lines += var_line + "\n"
            msg_line += var_lines.strip() + ");\n\n"
            msg_lines += msg_line
        return msg_lines


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

    def get_message_enum(self):
        enum_lines = ''
        enum_index_lines = ''

        for message in self.blackboard.messages:
            enum_line = self.MESSAGE_ENUM_LINE_FMT.format(
                nameStr=message.get_full_name(),
                msgId=" = 0x{:3X}".format(message.id))

            enum_lines += enum_line

        index = 0
        for message in self.blackboard.messages:
            enum_line = self.MESSAGE_ENUM_INDEX_LINE_FMT.format(
                nameStr=message.get_full_name(),
                index=" = " + str(index))

            enum_index_lines += enum_line
            index += 1

        enum_text = self.MESSAGE_ENUM_LINES_FMT.format(
            enumLines=enum_lines,
            enumIndexLines=enum_index_lines)

        return enum_text

    BB_ACCESSOR_FMT = """        public {varGetterType} {varName}
        {{
            get {{ return ({varGetterType})GetVariable(CanMessageIndex.{msgName}, {varIndex}).{varAccessor}; }}
            set {{ ProcessSetVariable(CanMessageIndex.{msgName}, {varIndex}, ({varSetterType})value); }}
        }}
"""

    def get_bb_accessors(self):
        accessors_text = ''
        for variable in self.blackboard.variables:
            msg_name = variable.message.get_full_name()
            var_name = variable.get_full_name()
            var_index = variable.msg_subindex
            var_getter_type = ''
            var_setter_type = ''
            var_accessor = ''
            if (variable.resolution_bits > 0):
                var_getter_type = 'float'
                var_setter_type = 'float'
                var_accessor = 'FloatValue'
            else:
                if(variable.is_signed):
                    if (variable.size_bits == 8):
                        var_getter_type = 'sbyte'
                    elif (variable.size_bits == 16):
                        var_getter_type = 'short'
                    elif (variable.size_bits == 32):
                        var_getter_type = 'int'
                    var_setter_type = 'int'
                    var_accessor = 'Value'
                else:
                    if (variable.size_bits == 8):
                        var_getter_type = 'byte'
                    elif (variable.size_bits == 16):
                        var_getter_type = 'ushort'
                    elif (variable.size_bits == 32):
                        var_getter_type = 'uint'
                    var_setter_type = 'uint'
                    var_accessor = 'UnsignedValue'

            accessors_text += self.BB_ACCESSOR_FMT.format(
                varName=var_name,
                msgName=msg_name,
                varIndex=var_index,
                varGetterType=var_getter_type,
                varSetterType=var_setter_type,
                varAccessor=var_accessor)
        return accessors_text
