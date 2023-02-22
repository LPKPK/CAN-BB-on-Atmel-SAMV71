"""Blackboard Class

Copyright (c) 2021 Sunrise Labs, Inc.
All rights reserved.
"""

from .BBUtils import BBUtils
from .Section import Section
from .SectionFiles import SectionFiles
from .Message import Message
from .TxSpec import TxSpec
from .RxSpec import RxSpec
from .Variable import Variable
from .Callback import Callback


class Blackboard:
    """Contains info about a blackboard as well as generated meta data to be used as content within
    various files in the system

    """

    def __init__(self, bb_json=None, section_files=None):
        """Constructor that takes a blackboard JSON-decoded object and converts it into this Blackboard
        object

        """
        self.section_files = []
        self.can_specs = []
        self.can_spec_map = {}
        self.sections = []
        self.messages = []
        self.variables = []
        self.base_dir = ''

        if bb_json is not None:
            self.from_json(bb_json, section_files)

    def from_json(self, bb_json, section_files):
        """Converts the given blackboard JSON-decoded associative array-based object into this parsed
        Blackboard instance.

        """

        # First obtain the section files if they were not given to us (assumes they are in the bb_json file)
        if not section_files:
            self.section_files = SectionFiles(bb_json).section_files
        else:
            self.section_files = section_files

        # Next go through the sections in the blackboard and build up the messages and variables
        sections = Blackboard.check_map(bb_json, "sections", "blackboard")
        msg_index = 0
        for section in sections:
            section_name = Blackboard.check_map(section, "name", "blackboard")
            section_obj = Section(section_name)
            self.sections.append(section_obj)

            for message in Blackboard.check_map(section, "messages", "section {}".format(section_obj.name)):
                message_name = Blackboard.check_map(message, "name", "section {}".format(section_obj.name))

                tx_channels = Blackboard.check_map(message, "tx", "tx in section {}".format(section_obj.name))
                tx_obj = TxSpec(tx_channels)
                rx_sections = Blackboard.check_map(message, "rx", "section {}".format(section_obj.name))
                rx_obj = RxSpec(rx_sections)

                callback_objs = []
                callbacks = Blackboard.check_map(message, "callbacks", "message {}".format(message_name))
                if callbacks:
                    for callback in callbacks:
                        callback_section_name = Blackboard.check_map(callback, "section",
                                                            "callback in message {}".format(message_name))
                        fn = Blackboard.check_map(callback, "fn", "callback in message {}".format(message_name))
                        try:
                            param = Blackboard.check_map(callback, "param",
                                                         "callback in message {}".format(message_name))
                        except Exception:
                            param = None
                        callback_obj = Callback(callback_section_name, fn, param)
                        callback_objs.append(callback_obj)

                message_obj = Message(
                    msg_index,
                    section_name,
                    message_name,
                    Blackboard.check_map(message, "description", "message {}".format(message_name)),
                    int(Blackboard.check_map(message, "id", "message {}".format(message_name)), 16),
                    Blackboard.check_map(message, "periodMs", "message {}".format(message_name)),
                    tx_obj,
                    rx_obj,
                    callback_objs)

                self.messages.append(message_obj)
                section_obj.messages.append(message_obj)
                msg_index += 1

                msg_subindex = 0
                offset_bits = 0
                for variable in message["variables"]:
                    variable_name = Blackboard.check_map(variable, "name",
                                                         "variable name in message {}".format(message_obj.name))

                    variable_obj = Variable(
                        msg_subindex,
                        message_obj,
                        variable_name,
                        Blackboard.check_map(variable, "description", "variable {}".format(variable_name)),
                        Blackboard.check_map(variable, "size", "variable {}".format(variable_name)),
                        offset_bits,
                        Blackboard.check_map(variable, "units", "variable {}".format(variable_name)),
                        # Default is unsigned.  Only signed if specified and True
                        # Missing:      signed
                        # Signed=true:  signed
                        # Signed=false: unsigned
                        False if ("signed" in variable and variable["signed"] is False) else True,
                        Blackboard.check_map(variable, "resolutionBits", "variable {}".format(variable_name)))

                    self.variables.append(variable_obj)
                    offset_bits += variable_obj.size_bits
                    message_obj.variables.append(variable_obj)
                    msg_subindex += 1

    @staticmethod
    def check_map(mapping, key, context):
        """Util method for getting a value within a dictionary by its key and raising an exception with
        the given context if it is not found.

        """
        return BBUtils.check_map(mapping, key, context)

    def __str__(self):
        """Conversion to string"""
        messages_str = ''
        variables_str = ''
        msg_cnt = 1
        for message in self.messages:
            messages_str = ''.join([messages_str, str(msg_cnt), ": ", "Message {}".format(message), '\n'])
            for variable in message.variables:
                messages_str = ''.join([messages_str, "      Variable {}".format(variable), '\n'])
            msg_cnt += 1
        var_cnt = 1
        for variable in self.variables:
            variables_str = ''.join([variables_str, str(var_cnt), ": ", "Variable {}".format(variable), '\n'])
            var_cnt += 1
        final_str = ''.join([
            "********************************************************************************\n",
            "Messages\n",
            "********************************************************************************\n",
            messages_str,
            "\n",
            "********************************************************************************\n",
            "Variables\n",
            "********************************************************************************\n",
            variables_str
        ])
        return final_str
