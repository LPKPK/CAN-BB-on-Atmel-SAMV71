"""Variable Class

Copyright (c) 2021 Sunrise Labs, Inc.
All rights reserved.
"""

from .Message import Message


class Variable:
    """Represents a variable within a CAN message"""

    def __init__(
            self,
            msg_subindex: int,
            message: Message,
            name: str,
            description: str,
            size_bits: int,
            offset_bits: int,
            units: str,
            is_signed: bool,
            resolution_bits: int):
        """Constructor

        :param msg_subindex int the subindex of this variable in its containing Variable

        :param message Message a reference to the containing Message for this Variable

        :param name str the base name of this variable.  Is not prefixed with the Section name.

        :param description str text description of this variable to be included in generated
         code as a comment.

        :param size_bits int the size of this variable in bits

        :param offset_bits int the bit-offset of this variable within its Message.  This along
         with the size must form a consistent string of Variables within a Message.

        :param units str a string representing the units of the Variable.  Helpful for generated
         documentation.

        :param is_signed bool true if this Variable represents a signed number.  Not applicable if
         resolution_bits is non-zero since floats are always signed.

        :param resolution_bits int the number of fixed-point bits of precision.  If non-zero,
         the "normal" type of this Variable becomes float.  For example, a value of 4 represents
         "Q4" numbers, which provide 4 bits (or 1/16) resolution.

        """

        self.msg_subindex = msg_subindex
        self.message = message
        self.name = name
        self.description = description
        self.size_bits = size_bits
        self.offset_bits = offset_bits
        self.units = units
        self.is_signed = is_signed
        self.resolution_bits = resolution_bits

        if ' ' in self.name:
            raise Exception("Variable name cannot contain spaces: {}".format(self.name))

    def get_value_type(self):
        """Retrieves the C/C++ data type for the internal value, e.g. int32_t, uint8_t, uint16_t, etc.

        """
        value_type_str = "int{:d}_t".format(self.size_bits)
        if not self.is_signed:
            value_type_str = "u" + value_type_str
        return value_type_str

    def get_normal_value_type(self):
        """Retrieves the "normal" C/C++ data type for the internal value.  This is either the same as
        get_value_type() or float if resolution_bits are non-zero.

        """
        normal_value_type_str = self.get_value_type()
        if self.resolution_bits > 0:
            normal_value_type_str = "float32_T"
        return normal_value_type_str

    def get_full_name(self):
        """Retrieves the variable name prefixed with the containing Message's section name.

        """
        return self.message.section_name + '_' + self.name

    def get_size_bytes(self) -> int:
        """Calculates and returns the size of the variable in bytes"""
        return self.size_bits // 8

    def get_offset_bytes(self) -> int:
        """Calculates and returns the offset of the variable in bytes"""
        return self.offset_bits // 8

    def get_c_selector(self) -> str:
        """Retrieves the internal data access "selector" for this Variable.  This is so we know which
        function to use to merge bits into the underlying blackboard element storage variable.
        """
        if self.is_signed:
            selector = "int" + str(self.size_bits)
        else:
            selector = "uint" + str(self.size_bits)
        return selector

    def get_return_statement(self) -> str:
        """Retrieves a C/C++ statement of code to return the value of this Variable based on its
        "normal" type.  Variables with non-zero resolution_bits are converted to float.
        """
        return_statement = "return value;"
        if self.resolution_bits > 0:
            return_statement = "return bb_utils_fixed{}ToFloat(value, {}U);".format(self.size_bits,
                                                                                    self.resolution_bits)
        return return_statement

    def get_convert_data_expression(self):
        """Retrieves a C/C++ expression of code to convert the value either directly to its native type
        (no-op) or to convert from float if resolution_bits are non-zero.

        """
        convert_data_expression = "value"
        if self.resolution_bits > 0:
            convert_data_expression = "bb_utils_floatToFixed{}(value, {}U)".format(self.size_bits, self.resolution_bits)
        return convert_data_expression
