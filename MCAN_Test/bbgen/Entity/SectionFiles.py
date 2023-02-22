"""Blackboard Section Files Class

Copyright (c) 2021 Sunrise Labs, Inc.
All rights reserved.
"""

from .BBUtils import BBUtils
from .SectionFile import SectionFile


class SectionFiles:
    """Class representing a group of section file entries and functions for handling them"""

    def __init__(self, bb_json: dict = None):
        self.section_files = []
        if bb_json:
            self.parse_section_files(bb_json)

    def parse_section_files(self, bb_json: dict) -> None:
        """
        Iterates over the section files found in the given dictionary and creates SectionFile objects for each entry
        :param bb_json: The dictionary created from the json section file
        """
        sections_files = BBUtils.check_map(bb_json, "sectionFiles", "blackboard")
        for sections_file in sections_files:
            section_name = BBUtils.check_map(sections_file, "section", "blackboard")
            section_file_list = BBUtils.check_map(sections_file, "files", "blackboard")
            for section_file in section_file_list:
                file_description = BBUtils.check_map(section_file, "description", "blackboard")
                file_type = BBUtils.check_map(section_file, "type", "blackboard")
                file_path = BBUtils.check_map(section_file, "path", "blackboard")

                self.section_files.append(SectionFile(section_name, file_description, file_type, file_path))
