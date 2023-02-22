"""File Munger Class

Copyright (c) 2021 Sunrise Labs, Inc.
All rights reserved.
"""

import os


class FileMunger:
    """Munges text into a given file"""

    def __init__(self, base_dir, file_name):
        """
        Constructor.
        :param base_dir str Root director for the given file
        :param file_name relative file_name to the given base_dir
        """
        self.base_dir = base_dir
        self.file_name = file_name

    # Base autogen sentry
    AUTOGEN_SENTRY = "// *** AUTOGENERATED BY BBGEN.  DO NOT EDIT THIS SECTION: "

    def munge_file(self, tag: str, updated_content: str) -> None:
        """
        Replaces all text within the specified autogen sentry, based on the given tag, within the
        file with the given updated text.
        :param tag str suffix to the AUTOGEN_SENTRY to find within the file
        :param updated_content str new content to replace in the file between the tagged sentry
        """
        sentry_count = 0
        full_path = os.path.join(self.base_dir, self.file_name)
        try:
            with open(full_path) as f:
                # print("Processing file {} of type {}".format(full_path, tag))
                lines = f.readlines()

                mod_lines = []
                munge = False
                sentry = FileMunger.AUTOGEN_SENTRY + tag
                for line in lines:
                    if sentry in line:
                        sentry_count += 1
                        if not munge:
                            mod_lines.append(line)
                        munge = not munge
                        if munge:
                            mod_lines.append(updated_content)

                    if not munge:
                        mod_lines.append(line)

                # Only expect to find exactly two of a given tagged sentry within a single file
                if sentry_count != 2:
                    raise Exception(
                        "Did not find proper autogen sentries (found {} expected 2) within {} like this: {}".format(
                            sentry_count, full_path, sentry))

                try:
                    with open(full_path, 'w') as f_write:
                        f_write.writelines(mod_lines)
                except OSError as we:
                    print("WARNING: Could not write {0} for munging: {1}".format(full_path, we))

        except OSError as re:
            print("WARNING: Could not read {0} for munging: {1}".format(full_path, re))
