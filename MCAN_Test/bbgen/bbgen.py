"""Main function entry point for blackboard code generation

Copyright (c) 2021 Sunrise Labs, Inc.
All rights reserved.
"""

# Internal imports
from Entity.Blackboard import Blackboard
from Entity.SectionFiles import SectionFiles

from Formatter.CCanSpecDeclFormatter import CCanSpecDeclFormatter
from Formatter.CCanSpecDefFormatter import CCanSpecDefFormatter
from Formatter.CCanSpecSchedDefFormatter import CCanSpecSchedDefFormatter
from Formatter.CBlackboardDeclFormatter import CBlackboardDeclFormatter
from Formatter.CBlackboardDefFormatter import CBlackboardDefFormatter
from Formatter.CDefaultCallbackDefFormatter import CDefaultCallbackDefFormatter
from Formatter.CDefaultCallbackDeclFormatter import CDefaultCallbackDeclFormatter
from Formatter.CppBlackboardDeclFormatter import CppBlackboardDeclFormatter
from Formatter.CppBlackboardDefFormatter import CppBlackboardDefFormatter

# System imports
import sys
import os
import json
import getopt


def usage(msg: str = None) -> None:
    """
    Prints usage with the optional given msg.
    :param msg: String to print with usage help
    :return: None
    """
    if msg is not None and len(msg) > 0:
        print(msg, '\n')
    print('usage: {} -j <blackboard JSON file>'.format(sys.argv[0]))


def check_param(param_name: str, param_value: str) -> None:
    """Checks if the given parameter is set.  Meant for w/ getopt"""
    if len(param_value) == 0:
        usage("\nMust specify {}".format(param_name))
        exit(-1)


def generate(bb_json_file_name: str, bb_section_file_name: str) -> None:
    """Generates code and updates files according to the given blackboard JSON file and 
    Blackboard section-files specifier JSON file.

    :param bb_json_file_name:str The name of the blackboard JSON file to use as the blackboard
    source.  All files within the blackboard JSON (e.g. within the "blackboardFiles" section) are
    relative to the path of this given file.

    :param bb_section_file_name:str A separate optional file containing the "sectionFiles"
    section that may otherwise be located in bb_json_file_name.

    """
    # Convert the given file name into a Blackboard object.
    with open(bb_json_file_name, 'r') as bb_json_file:
        bb_json = bb_json_file.read()

    section_files = None
    if bb_section_file_name:
        with open(bb_section_file_name, 'r') as bb_section_file:
            section_file_json = bb_section_file.read()
            section_files_obj = json.loads(section_file_json, strict=False)
            section_files = SectionFiles(section_files_obj).section_files

    bb_obj = json.loads(bb_json, strict=False)
    bb = Blackboard(bb_obj, section_files)
    bb.base_dir = os.path.dirname(bb_section_file_name)

    formatter_map = {
        "CAN_SPEC_C_DECL": CCanSpecDeclFormatter("CAN_SPEC_C_DECL", bb),
        "CAN_SPEC_C_DEF": CCanSpecDefFormatter("CAN_SPEC_C_DEF", bb),
        "CAN_SPEC_SCHED_C_DEF": CCanSpecSchedDefFormatter("CAN_SPEC_SCHED_C_DEF", bb),
        "BB_C_DECL": CBlackboardDeclFormatter("BB_C_DECL", bb),
        "BB_C_DEF": CBlackboardDefFormatter("BB_C_DEF", bb),
        "BB_CPP_DECL": CppBlackboardDeclFormatter("BB_CPP_DECL", bb),
        "BB_CPP_DEF": CppBlackboardDefFormatter("BB_CPP_DEF", bb),
        "BB_C_DEFAULT_CB": CDefaultCallbackDefFormatter("BB_C_DEFAULT_CB", bb),
        "BB_C_CALLBACK_DECL": CDefaultCallbackDeclFormatter("BB_C_CALLBACK_DECL", bb)
    }

    for section_file in bb.section_files:
        formatter = formatter_map[section_file.type]
        formatter.format(section_file)


def main():
    """Main entry point."""

    bb_json_file_name = ''
    bb_section_file_name = None

    try:
        opts, args = getopt.getopt(sys.argv[1:], "j:s:")
    except getopt.GetoptError as err:
        usage()
        print(str(err))
        sys.exit(2)

    for o, a in opts:
        if o == "-j":
            bb_json_file_name = a
        if o == "-s":
            bb_section_file_name = a

    check_param("bb_json_file_name", bb_json_file_name)

    generate(bb_json_file_name, bb_section_file_name)


if __name__ == "__main__":
    main()
