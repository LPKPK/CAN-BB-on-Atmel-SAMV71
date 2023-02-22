"""Callback Class

Class representing a callback on a CAN message

Copyright (c) 2021 Sunrise Labs, Inc.
All rights reserved.
"""


class Callback:
    """Class representing a callback on a CAN message"""
    def __init__(self, section_name=None, fn=None, param=None):
        self.section_name = section_name
        self.fn = fn
        self.param = param

    def get_c_fn(self) -> str:
        """
        Formats the callback function in C
        :return: Formatted string of C code if the callback has a function - otherwise NULL
        """
        return "&{}".format(self.fn) if self.fn else "NULL"

    def get_cpp_fn(self) -> str:
        """
        Formats the callback function in C++
        :return: Formatted string of C++ code if the callback has a function - otherwise nullptr
        """
        return self.fn if self.fn else "nullptr"

    def get_c_param(self) -> str:
        """
        Formats the callback parameter in C
        :return: Formatted string of C code if the callback has a parameter - otherwise NULL
        """
        return "(void *)({})".format(self.param) if self.param else "NULL"

    def get_cpp_param(self) -> str:
        """
        Formats the callback parameter in C++
        :return: Formatted string of C++ code if the callback has a parameter - otherwise nullptr
        """
        return self.param if self.param else "nullptr"
