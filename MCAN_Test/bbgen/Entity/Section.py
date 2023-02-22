"""Blackboard Section Class

Copyright (c) 2021 Sunrise Labs, Inc.
All rights reserved.
"""


class Section:
    """Represents a section within the Blackboard model."""

    def __init__(self, name):
        """Constructor

        :param name str the name of the section, which serves primarily as a prefix for message
         names within that section.

        """
        self.name = name
        self.messages = []

    def __str__(self):
        """Conversion to string."""
        return self.name
