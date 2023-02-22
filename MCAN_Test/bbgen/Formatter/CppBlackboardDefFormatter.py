"""C++ Blackboard Definition Formatter Class

Formatter class for the blackboard function definitions in C++

Copyright (c) 2021 Sunrise Labs, Inc.
All rights reserved.
"""

from .BaseFormatter import BaseFormatter
from .CppFormatterSnippets import CppFormatterSnippets

# TODO: rework import/module approach
import sys
import os
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from FileMunger import FileMunger
from Entity.SectionFile import SectionFile
from Entity.Message import Message
from Entity.Variable import Variable


class CppBlackboardDefFormatter(BaseFormatter):
    """Formatter subclass for the blackboard function definitions in C++"""

    def __init__(self, tag, blackboard):
        super().__init__(tag, blackboard)
        self.def_list = ''

    def format(self, section_file: SectionFile) -> None:
        """
        Formats the blackboard definition string using the given section file object

        :param section_file: The section file to get the information to format
        """
        # Setup the formatted enum
        self.def_list = ''
        
        # Use base class to filter only applicable messages which either tx or rx to/from this section
        super().filter_applicable_variables(section_file)

        # Setup the full snippet
        
        bb_def = CppFormatterSnippets.BB_DEF_FMT.format(
            canSpecDesc=section_file.description,
            canSpecName=section_file.section,
            canSpecType=section_file.type,
            bbDefs=self.def_list)

        # Replace the content in the file w/ the newly formatted content
        fm = FileMunger(self.blackboard.base_dir, section_file.path)
        fm.munge_file(self.tag, bb_def)

    def filter_applicable_variable(
            self,
            section_file: SectionFile,
            message: Message,
            variable: Variable,
            do_rx: bool = None,
            do_tx: bool = None) -> None:
        """
        Formats the blackboard function definitions for getting and setting each variable
        :param section_file: The section file to use
        :param message: The message object to use
        :param variable: The variable object that matched the filter
        :param do_rx: If True, use the Rx CAN Channel - otherwise, the channel is none
        :param do_tx: If True, use the Tx Channels from the message - otherwise, the channels are none
        """
        
        # Prefix each line in the description so the doxy comment looks right
        prefixed_description = " * ".join(variable.description.splitlines(True))
        getter_def_text = CppFormatterSnippets.GETTER_DEF_FMT.format(
            baseName=variable.get_full_name(),
            description=prefixed_description,
            valueType=variable.get_value_type(),
            units=variable.units,
            normalValueType=variable.get_normal_value_type())

        setter_def_text = CppFormatterSnippets.SETTER_DEF_FMT.format(
            baseName=variable.get_full_name(),
            valueType=variable.get_value_type(),
            units=variable.units,
            normalValueType=variable.get_normal_value_type())

        self.def_list += (getter_def_text + setter_def_text)
