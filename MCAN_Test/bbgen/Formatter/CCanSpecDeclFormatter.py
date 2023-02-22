"""C CAN Specialization Declaration Formatter Class

Formatter class for the CAN specialization declarations in C

Copyright (c) 2021 Sunrise Labs, Inc.
All rights reserved.
"""

from .BaseFormatter import BaseFormatter
from .CFormatterSnippets import CFormatterSnippets

# TODO: rework import/module approach
import sys
import os

sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from FileMunger import FileMunger
from Entity.SectionFile import SectionFile
from Entity.Message import Message


class CCanSpecDeclFormatter(BaseFormatter):
    """Formatter subclass for the CAN specialization declarations in C"""
    def __init__(self, tag, blackboard):
        super().__init__(tag, blackboard)
        self.enum_list = ''

    def format(self, section_file: SectionFile) -> None:
        """
        Formats the CAN specialization declaration string using the given section file object

        :param section_file: The section file to get the information to format
        """
        # Setup the formatted enum
        self.enum_list = ''

        # Use base class to filter only applicable messages which either tx or rx to/from this section
        super().filter_applicable_messages(section_file)

        # Setup the full snippet
        can_spec_decl = CFormatterSnippets.CAN_SPEC_DECL_FMT.format(
            canSpecDesc=section_file.description,
            canSpecName=section_file.section,
            canSpecType=section_file.type,
            canSpecEnum=self.enum_list)

        # Replace the content in the file w/ the newly formatted content
        fm = FileMunger(self.blackboard.base_dir, section_file.path)
        fm.munge_file(self.tag, can_spec_decl)

    def filter_applicable_message(
            self,
            section_file: SectionFile,
            message: Message,
            do_rx: bool = None,
            do_tx: bool = None) -> None:
        """
        Formats messages that match the filter
        :param section_file: The section file to use
        :param message: The message object that matched the filter
        :param do_rx: If True, use the Rx CAN Channel - otherwise, the channel is none
        :param do_tx: If True, use the Tx Channels from the message - otherwise, the channels are none
        """
        self.enum_list += CFormatterSnippets.CAN_SPEC_ENUM_ROW_FMT.format(
            enumName=message.get_full_name())
