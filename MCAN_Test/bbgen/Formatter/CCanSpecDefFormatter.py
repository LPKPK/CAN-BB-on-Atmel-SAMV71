"""C CAN Specialization Definition Formatter Class

Formatter class for the CAN specialization definition in C

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


class CCanSpecDefFormatter(BaseFormatter):
    """Formatter subclass for the CAN specialization Definition in C"""
    def __init__(self, tag, blackboard):
        super().__init__(tag, blackboard)
        self.def_list = ''

    def format(self, section_file: SectionFile) -> None:
        """
        Formats the CAN specialization definition string using the given section file object

        :param section_file: The section file to get the information to format
        """
        # Setup the formatted enum
        self.def_list = ''

        # Use base class to filter only applicable messages which either tx or rx to/from this section
        super().filter_applicable_messages(section_file)

        # Setup the full snippet
        can_spec_def = CFormatterSnippets.CAN_SPEC_DEF_FMT.format(
            canSpecDesc=section_file.description,
            canSpecName=section_file.section,
            canSpecType=section_file.type,
            canSpecTableRows=self.def_list)

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
        var_map_rows = ''
        last_start_byte = 0
        for var in message.variables:
            last_start_byte = var.get_offset_bytes()
            var_map_rows += CFormatterSnippets.CAN_SPEC_DEF_VAR_MAP_ROW_FMT.format(
                variableName='"{}",'.format(var.name),
                signSpec="CAN_DATA_SIGNED," if var.is_signed else "CAN_DATA_UNSIGNED,",
                startByte=str(last_start_byte) + ",",
                byteCount=var.get_size_bytes())
        num_blanks = 4 - len(message.variables)
        for _ in range(num_blanks):
            var_map_rows += CFormatterSnippets.CAN_SPEC_DEF_VAR_MAP_ROW_FMT.format(
                variableName="NULL_STR,",
                signSpec="CAN_DATA_UNSIGNED,",
                startByte=str(last_start_byte) + ",",
                byteCount=0)

        tx_channels = "CAN_NONE" if not do_tx else message.get_c_tx_channels()
        rx_sections = "CAN_CHAN1" if do_rx else "CAN_NONE"

        callback_obj = Callback()
        for callback in message.callbacks:
            if callback.section_name == section_file.section:
                callback_obj = callback
                break

        self.def_list += CFormatterSnippets.CAN_SPEC_DEF_ROW_FMT.format(
            msgId=message.id,
            messageName='"{}"'.format(message.get_full_name()),
            periodMs=message.period_ms,
            dataLength=message.get_size_bytes(),
            varMapRows=var_map_rows,
            dataRows=var_map_rows,
            callback=callback_obj.get_c_fn(),
            callbackContext=callback_obj.get_c_param(),
            txChannels=tx_channels,
            rxSections=rx_sections
        )

    def filter_applicable_variable(
            self,
            section_file: SectionFile,
            message: Message,
            variable: Variable,
            do_rx: bool = None,
            do_tx: bool = None) -> None:
        """
        Overrides base class - variables are handled above so this function is a no-op
        :param section_file: The section file to use
        :param message: The message object to use
        :param variable: The variable object that matched the filter
        :param do_rx: If True, use the Rx CAN Channel - otherwise, the channel is none
        :param do_tx: If True, use the Tx Channels from the message - otherwise, the channels are none
        """
        return
