"""Blackboard Section File Class

Copyright (c) 2021 Sunrise Labs, Inc.
All rights reserved.
"""


class SectionFile:
    """Class representing an entry in a section file"""
    def __init__(self, section: str, description: str, section_type: str, path: str):
        """
        Constructor
        :param section: String representing the section this entry is in
        :param description: Description of the entry
        :param section_type: String for the file munger to look for
        :param path: Path to the file for this entry
        """
        self.section = section
        self.description = description
        self.type = section_type
        self.path = path
