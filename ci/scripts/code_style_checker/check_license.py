#!/usr/bin/python

#  -------------------------------------------------------------------------
#  Copyright (C) 2020 BMW AG
#  -------------------------------------------------------------------------
#  This Source Code Form is subject to the terms of the Mozilla Public
#  License, v. 2.0. If a copy of the MPL was not distributed with this
#  file, You can obtain one at https://mozilla.org/MPL/2.0/.
#  -------------------------------------------------------------------------

import re, string
from common_modules.common import *


# generate license regexes only once
G_RE_LICENSE_TEMPLATE_OPEN = re.compile(r"""(?://|::|#)  -------------------------------------------------------------------------
(?://|::|#)  Copyright \(C\) 2\d{3}(?:-2\d{3})? BMW AG$(?:\n(?://|::|#)  Copyright \(C\) .*$)*
(?://|::|#)  -------------------------------------------------------------------------
(?://|::|#)  This Source Code Form is subject to the terms of the Mozilla Public
(?://|::|#)  License, v\. 2\.0\. If a copy of the MPL was not distributed with this
(?://|::|#)  file, You can obtain one at https://mozilla\.org/MPL/2\.0/\.
(?://|::|#)  -------------------------------------------------------------------------
""", re.MULTILINE)


def check_specific_license_in_file(filename, file_contents, license_re):
    """
    Checks if the file contains a valid license according to the license template provided
    """
    m = license_re.search(file_contents)
    if not m:
        return False

    text_before_license = file_contents[:m.span()[0]]
    if len(text_before_license.split('\n')) > 3:
        return False

    return True


def check_license_for_file(file_name, file_contents, solution_path):
    """
    Check license for given file name.
    """

    license_re = G_RE_LICENSE_TEMPLATE_OPEN

    if not check_specific_license_in_file(file_name, file_contents, license_re):
        log_warning("check_license_in_file", file_name, 1, "no valid license found")


def main():
    targets = sys.argv[1:]
    targets = get_all_files(targets)

    if len(targets) == 0:
        print("""
\t**** No input provided ****
\tTakes a list of files/directories as input and performs specific style checking on all files/directories.

\tGives warnings if the file does not contain a valid license text. It does not check if Copyright statements are included.
""")
        exit(0)

    path = os.path.realpath(os.path.join(os.path.dirname(os.path.realpath(__file__)), "..", ".."))
    for t in targets:
        file_contents, _ = read_file(t)
        check_license_for_file(t, file_contents, path)

if __name__ == "__main__":
    main()
