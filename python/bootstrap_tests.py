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
import time
import optparse
import os
import subprocess

from colors import colors
from stdout_comparator import StdoutComparator

## -----------------------------------------------------------------------------

PYTHON = 'python'
PYTA_PATH='./python/pyta.py'
PYTHON_TESTS_DIR = './python/tests/'
INTERMEDIATE_EXTENSION = '.tmp.py'

## -----------------------------------------------------------------------------

TESTS_BLACKLIST = [
    './python/tests/debug_test.py',
    # TODO: [COREVM-486] Un-blacklist Python test `python/tests/chr.py`
    './python/tests/chr.py'
]

## -----------------------------------------------------------------------------

def pyta_cmdl_args(path, options):
    args = [PYTHON, PYTA_PATH, path]

    if options.sanity_test:
        args.append('--sanity-test')
    elif options.dynamic_analysis:
        args.append('--dynamic-analysis')

    return args

## -----------------------------------------------------------------------------

def python_cmdl_args(path):
    return [PYTHON, path]

## -----------------------------------------------------------------------------

def run(options):
    """Return a boolean value indicating if the run succeeds."""
    inputs = glob.glob(PYTHON_TESTS_DIR + '*.py') \
        if not options.tests else options.tests

    real_inputs = [
        path for path in inputs if not path.endswith(INTERMEDIATE_EXTENSION)
    ]

    real_inputs = set(real_inputs) - set(TESTS_BLACKLIST)

    print 'Bootstrapping Python tests...'
    if options.python_only:
        print '(Python only)'
    elif options.sanity_test:
        print '(Sanity test)'
    elif options.dynamic_analysis:
        print '(Dynamic analysis)'

    if options.strict_mode:
        print '[strict mode]'

    print 'Testing using the following %d input file(s):' % len(real_inputs)

    # Bring blank line.
    print

    cumulated_time = 0.0
    passed_tests_count = 0
    total_tests_count = len(real_inputs)

    SUCCESS = 'SUCCESS'
    FAILED = 'FAILED'

    end_color = colors.ENDC if options.show_color else ''

    for path in real_inputs:
        info = path

        lhs_args = python_cmdl_args(path) \
            if options.python_only else pyta_cmdl_args(path, options)
        rhs_args = python_cmdl_args(path)

        begin_time = time.time()

        if not (options.sanity_test or options.dynamic_analysis):
            retcode = StdoutComparator(lhs_args, rhs_args).run(show_color=options.show_color)
        else:
            retcode = subprocess.call(lhs_args, stdout=subprocess.PIPE)

        end_time = time.time()

        elapsed_time = end_time - begin_time

        cumulated_time += elapsed_time

        success = retcode == 0

        if success:
            passed_tests_count += 1

        status_str = SUCCESS if success else FAILED
        begin_color = (colors.OKGREEN if success else colors.FAIL) if options.show_color else ''

        print '{path:<30}{begin_color} [{status_str:<7}] {elapsed_time:3.3f} s{end_color}'.format(
            path=path,
            begin_color=begin_color,
            status_str=status_str,
            elapsed_time=elapsed_time,
            end_color=end_color)

    # Print overall status.
    success = passed_tests_count == total_tests_count
    print
    begin_color = (colors.OKGREEN if success else colors.FAIL) if options.show_color else ''
    print '{begin_color}{success}{end_color}'.format(
        begin_color=begin_color,
        success=SUCCESS if success else FAILED,
        end_color=end_color)
    print '%d / %d tests passed' % (passed_tests_count, total_tests_count)
    print 'Cumulated execution time: {cumulated_time:3.3f} s'.format(
        cumulated_time=cumulated_time)

    return success if options.strict_mode else True

## -----------------------------------------------------------------------------

def main():
    parser = optparse.OptionParser(
        usage='usage: %prog [options]',
        version='%prog v0.1.1')

    parser.add_option(
        '-p',
        '--python-only',
        action='store_true',
        dest='python_only',
        default=False,
        help='Run Python only'
    )

    parser.add_option(
        '-t',
        '--test',
        action='append',
        dest='tests',
        default=[],
        help='One or more file(s) to test'
    )

    parser.add_option(
        '-s',
        '--sanity-test',
        action='store_true',
        dest='sanity_test',
        default=False,
        help='Sanity test only'
    )

    parser.add_option(
        '-a',
        '--dynamic-analysis',
        action='store_true',
        dest='dynamic_analysis',
        default=False,
        help='Enable dynamic analysis'
    )

    parser.add_option(
        '-x',
        '--strict-mode',
        action='store_true',
        dest='strict_mode',
        default=False,
        help='Run in strict mode (all tests must pass)'
    )

    parser.add_option(
        '-c',
        '--color',
        action='store_true',
        dest='show_color',
        default=False,
        help='Whether to show colored output'
    )

    options, _ = parser.parse_args()

    if options.python_only and options.sanity_test:
        sys.stderr.write('Cannot specify --python-only and --sanity-test togeter.\n')
        sys.exit(-1)

    if options.python_only and options.dynamic_analysis:
        sys.stderr.write('Cannot specify --python-only and --dynamic-analysis together.\n')
        sys.exit(-1)

    if options.sanity_test and options.dynamic_analysis:
        sys.stderr.write('Cannot specify --sanity-test and --dynamic-analysis together.\n')
        sys.exit(-1)

    # Success boolean to exit code conversion.
    return 0 if run(options) else 1

## -----------------------------------------------------------------------------

if __name__ == '__main__':
    import sys
    sys.exit(main())

## -----------------------------------------------------------------------------
