"""Message Class

Copyright (c) 2021 Sunrise Labs, Inc.
All rights reserved.
"""

from math import trunc

from typing import List

from .TxSpec import TxSpec
from .RxSpec import RxSpec
from .Callback import Callback


class Message:
    """Class to represent a CAN message"""

    def __init__(
            self,
            msg_index: int,
            section_name: str,
            name: str,
            description: str,
            msg_id: int,
            period_ms: int,
            tx: TxSpec,
            rx: RxSpec,
            callbacks: List[Callback]):
        """Constructor

        :param msg_index int CAN message index with can_msg_table

        :param section_name str the name of the section this message is within.  To be used as a
        prefix for the full name.

        :param name str the base name of the message

        :param description str an arbitrary description of the message

        :param msg_id int the CAN message identifier (11-bit) of this message

        :param period_ms int the number of milliseconds between planned transmissions of this message

        :param tx TxSpec the TxSpec describing on which channels (if any) on which node this
        message is sent

        :param rx RxSpec the RxSpec describing on which nodes this message is to be received

        :param callbacks Callback[] an array of Callback objects for specializations of this message.

        """
        self.msg_index = msg_index
        self.section_name = section_name
        self.name = name
        self.description = description
        self.id = msg_id
        self.period_ms = period_ms
        self.tx = tx
        self.rx = rx
        self.callbacks = callbacks
        self.variables = []

        if ' ' in self.name:
            raise Exception("Message name cannot contain spaces: {}".format(self.name))

    def get_full_name(self) -> str:
        """
        Retrieves the message name prefixed with the section name
        :return: Formatted string containing the message name prefixed with the section name
        """
        return self.section_name + '_' + self.name

    def get_size_bytes(self) -> int:
        """
        Retrieves the total size of this message in bytes
        :return: Int representing the total size of this message in bytes
        """
        total_size_bits = 0
        for variable in self.variables:
            total_size_bits += variable.size_bits
        return trunc(total_size_bits / 8)

    def get_c_tx_channels(self) -> str:
        """Retrieves a logic-OR collection of CanChannel values for each one specified in the TxSpec
        :return: Formatted string of C code containing each tx channel ORed together
        """
        if len(self.tx.channels) > 1:
            # C++ won't let you OR multiple enums together.  This is by far the easiest overall way
            # to deal with it.  But make these static casts the exception.  Very few messages are
            # transmitted on multiple busses.
            return "(CanChannel_T)(" + ' | '.join(('(uint16_t)(' + channel + ")") for channel in self.tx.channels) + ")"
        else:
            return ' | '.join((channel for channel in self.tx.channels))
