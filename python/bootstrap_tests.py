# The MIT License (MIT)

# Copyright (c) 2015 Yanzheng Li

# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

"""
Bootstrap Python tests.

Run as:

python python/bootstrap_tests.py
"""

import glob
import optparse
import os

from colors import colors
from stdout_comparator import StdoutComparator

## -----------------------------------------------------------------------------

PYTHON = 'python'
PYTA_PATH='./python/pyta.py'
PYTHON_TESTS_DIR = './python/tests/'
INTERMEDIATE_EXTENSION = '.tmp.py'

## -----------------------------------------------------------------------------

def pyta_cmdl_args(path, options):
    args = [PYTHON, PYTA_PATH, path]

    if options.format:
        args.extend(['--format', options.format])

    return args

## -----------------------------------------------------------------------------

def python_cmdl_args(path):
    return [PYTHON, path]

## -----------------------------------------------------------------------------

def run(options):
    inputs = glob.glob(PYTHON_TESTS_DIR + '*.py')
    real_inputs = [
        path for path in inputs if not path.endswith(INTERMEDIATE_EXTENSION)
    ]

    print 'Bootstrapping Python tests...'
    print 'Testing using the following %d input file(s):' % len(real_inputs)

    # Bring blank line.
    print

    for path in real_inputs:
        info = path

        lhs_args = python_cmdl_args(path) \
            if options.python_only else pyta_cmdl_args(path, options)
        rhs_args = python_cmdl_args(path)

        retcode = StdoutComparator(lhs_args, rhs_args).run()

        if retcode == 0:
            info += (colors.OKGREEN + ' [SUCCESS]' + colors.ENDC)
        else:
            info += (colors.FAIL + ' [FAILED]' + colors.ENDC)

        print info

## -----------------------------------------------------------------------------

def main():
    parser = optparse.OptionParser(
        usage='usage: %prog [options]',
        version='%prog v0.1')

    parser.add_option(
        '-t',
        '--format',
        action='store',
        dest='format',
        default='binary',
        help='Bytecode format (binary or text)'
    )

    parser.add_option(
        '-p',
        '--python-only',
        action='store_true',
        dest='python_only',
        default=False,
        help='Run Python only'
    )

    options, _ = parser.parse_args()

    run(options)

## -----------------------------------------------------------------------------

if __name__ == '__main__':
    main()

## -----------------------------------------------------------------------------
