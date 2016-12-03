# The MIT License (MIT)

# Copyright (c) 2015 Yanzheng Li

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import subprocess

from colors import colors


class StdoutComparator(object):

    def __init__(self, lhs_args, rhs_args):
        self.lhs_args = lhs_args
        self.rhs_args = rhs_args

    def run(self, show_color=True):
        try:
            lhs_output = subprocess.check_output(self.lhs_args)
        except subprocess.CalledProcessError:
            return -1

        try:
            rhs_output = subprocess.check_output(self.rhs_args)
        except subprocess.CalledProcessError:
            return -1

        if lhs_output == rhs_output:
            return 0

        output = ''
        if show_color:
            output = colors.HEADER

        output += 'Difference in output:\n'

        if show_color:
            output += colors.FAIL

        output += lhs_output
        output += '\n'

        if show_color:
            output += colors.OKBLUE
        output += rhs_output

        if show_color:
            output += colors.ENDC

        output += '\n'

        print output

        return -1
