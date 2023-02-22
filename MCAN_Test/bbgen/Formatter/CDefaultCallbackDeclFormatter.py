"""C Callback Declaration Formatter Class

Formatter class for the callback declarations in C

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
from Entity.Callback import Callback
from Entity.Message import Message
from Entity.Variable import Variable
from Entity.SectionFile import SectionFile


class CDefaultCallbackDeclFormatter(BaseFormatter):
    """Formatter subclass for the callback declarations in C"""

    def __init__(self, tag, blackboard):
        super().__init__(tag, blackboard)
        self.cb_list = ''

    def format(self, section_file: SectionFile) -> None:
        """
        Formats the callback declaration string using the given section file object

        :param section_file: The section file to get the information to format
        """
        # Setup the formatted enum
        self.cb_list = ''

        # Use base class to filter only applicable messages which either tx or rx to/from this section
        super().filter_applicable_messages(section_file)

        # Setup the full snippet
        can_spec_def = CFormatterSnippets.BB_DEFAULT_CB_DECL_FMT.format(
            canSpecName=section_file.section,
            callbackDecls=self.cb_list)

        # Replace the content in the file w/ the newly formatted content
        fm = FileMunger(self.blackboard.base_dir, section_file.path)
        fm.munge_file(self.tag, can_spec_def)

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
        callback_obj = Callback()
        for callback in message.callbacks:
            if callback.section_name == section_file.section:
                callback_obj = callback
                break
        if callback_obj.fn is not None:
            self.cb_list += CFormatterSnippets.BB_CALLBACK_DECL_FMT.format(
                callbackName=callback_obj.fn,
                callbackParam=callback_obj.param
            )

    def filter_applicable_variable(
            self,
            section_file: SectionFile,
            message: Message,
            variable: Variable,
            do_rx: bool = None,
            do_tx: bool = None) -> None:
        """
        Overrides base class - this class doesn't handle variables, just messages
        :param section_file: The section file to use
        :param message: The message object to use
        :param variable: The variable object that matched the filter
        :param do_rx: If True, use the Rx CAN Channel - otherwise, the channel is none
        :param do_tx: If True, use the Tx Channels from the message - otherwise, the channels are none
        """
        return
