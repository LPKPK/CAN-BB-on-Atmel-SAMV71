"""BaseFormatter Class

Parent class for all other formatters containing base functions for filtering
messages and variables

Copyright (c) 2021 Sunrise Labs, Inc.
All rights reserved.
"""

import abc
import sys
import os
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from Entity.Blackboard import Blackboard
from Entity.SectionFile import SectionFile
from Entity.Message import Message
from Entity.Variable import Variable


class BaseFormatter(abc.ABC):
    """Abstract base class for formatting parts of the Blackboard.

    Subclasses must implement the 'format' method. This class also provides some basic reusable
    services such as a hook for all applicable messages or variables.
    """

    def __init__(self, tag: str, blackboard: Blackboard) -> None:
        """
        Initializes the required formatter member variables
        :param tag: String used by the file munger to locate where to insert generated strings
        :param blackboard: Blackboard object to use when generating formatted strings
        """
        self.tag = tag
        self.blackboard = blackboard

    @abc.abstractmethod
    def format(self, section_file: SectionFile):
        """
        Implemented in subclasses
        :param section_file: The section file to use when formatting
        """
        pass

    def filter_applicable_messages(self, section_file: SectionFile, all_msgs: bool = False) -> None:
        """
        Calls the filter_applicable_message function on all messages that are transmitted or received
        by the section in the given section file
        :param section_file: The SectionFile object to use to filter the messages
        :param all_msgs: If True, send all messages through regardless of whether they are transmitted
        or received by the given section
        """
        # Only call function on messages that are being received or transmitted by this section
        for message in self.blackboard.messages:
            any_rx = any(rx_section == section_file.section for rx_section in message.rx.sections)
            any_tx = message.section_name == section_file.section

            if any_rx or any_tx or all_msgs:
                self.filter_applicable_message(section_file, message, any_rx, any_tx)

    def filter_applicable_message(
            self,
            section_file: SectionFile,
            message: Message,
            do_rx: bool = None,
            do_tx: bool = None) -> None:
        """
        Implemented in subclasses
        :param section_file: The section file to use
        :param message: The message object that matched the filter
        :param do_rx: If True, use the Rx CAN Channel - otherwise, the channel is none
        :param do_tx: If True, use the Tx Channels from the message - otherwise, the channels are none
        """
        pass

    def filter_applicable_variables(self, section_file: SectionFile) -> None:
        """
        Calls the filter_applicable_variable function on all variables within messages that are transmitted or received
        by the section in the given section file
        :param section_file: The SectionFile object to use to filter the variables
        """
        for message in self.blackboard.messages:
            any_rx = any(rx_section == section_file.section for rx_section in message.rx.sections)
            any_tx = message.section_name == section_file.section

            if any_rx or any_tx:
                for variable in message.variables:
                    self.filter_applicable_variable(section_file, message, variable, any_rx, any_tx)

    def filter_applicable_variable(
            self,
            section_file: SectionFile,
            message: Message,
            variable: Variable,
            do_rx: bool = None,
            do_tx: bool = None) -> None:
        """
        Implemented in subclasses
        :param section_file: The section file to use
        :param message: The message object to use
        :param variable: The variable object that matched the filter
        :param do_rx: If True, use the Rx CAN Channel - otherwise, the channel is none
        :param do_tx: If True, use the Tx Channels from the message - otherwise, the channels are none
        """
        pass
