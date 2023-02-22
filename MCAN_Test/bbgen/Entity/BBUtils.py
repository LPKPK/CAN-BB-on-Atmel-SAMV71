"""Blackboard Generation Utility Functions

Class containing blackboard generation utility functions

Copyright (c) 2021 Sunrise Labs, Inc.
All rights reserved.
"""


class BBUtils:
    """Class containing blackboard generation utility functions"""
    @staticmethod
    def check_map(mapping: dict, key: str, context: str) -> any:
        """
        Util method for getting a value within a dictionary by its key and raising an exception with
        the given context if it is not found.
        :param mapping: The dictionary containing the values
        :param key: The key to search for
        :param context: A string to print if the key is not found to assist with debugging
        :return: The value associated with the key in the dictionary
        """
        # Note: Blackboard 'groups' are an experimental method of having multiple instances of a given type of device
        # on a network.
        # Support for groups should be optional
        # If group size is missing, set it to 1, if the groupMask is missing set it to 0xF
        if (key == 'groupSize') and (key not in mapping):
            return 1
        if (key == 'groupMask') and (key not in mapping):
            return 0xF

        if key not in mapping:
            raise Exception("{} is not found within {}".format(key, context))
        return mapping[key]
