"""Rx Specification Class

Copyright (c) 2021 Sunrise Labs, Inc.
All rights reserved.
"""
from typing import List

from .SectionFile import SectionFile


class RxSpec:
    """Class to represent the RX specification for a given message"""

    def __init__(self, sections: List[SectionFile]):
        """
        Constructor
        :param sections SectionFile[] List of section files to use for the Rx specification
        """
        self.sections = sections
