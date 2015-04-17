"""
The MIT License (MIT)

Copyright (c) 2015 Yanzheng Li

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
"""

"""
Bootstrap Python tests.

Run as:

python python/bootstrap_tests.py
"""

import glob
import optparse
import os
import subprocess

from colors import colors
from stdout_comparator import StdoutComparator


PYTHON = 'python'
PYTHON_TESTS_DIR = './python/tests/'
PYTHON_COMPILER = './python/python_compiler.py'
PYTHON_CODE_TRANSFORMER = './python/code_transformer.py'
INFO_FILE = './info.json'
COREVM = './coreVM'
INTERMEDIATE_EXTENSION = '.tmp.py'
BYTECODE_EXTENSION = '.core'


def code_transformer_input_to_output_path(path):
    return os.path.splitext(path)[0] + INTERMEDIATE_EXTENSION


def compiler_input_to_output_path(path):
    return code_transformer_input_to_output_path(path) + BYTECODE_EXTENSION


def code_transformer_cmdl_args(path):
    return [
        PYTHON,
        PYTHON_CODE_TRANSFORMER,
        '--input',
        path,
        '--output',
        code_transformer_input_to_output_path(path)
    ]


def compiler_cmdl_args(path):
    return [
        PYTHON,
        PYTHON_COMPILER,
        '--input',
        code_transformer_input_to_output_path(path),
        '--info-file',
        INFO_FILE,
        '--output',
        compiler_input_to_output_path(path)
    ]


def corevm_cmdl_args(path):
    return [COREVM, '--input', compiler_input_to_output_path(path)]


def python_cmdl_args(path):
    return [PYTHON, path]


def run(options):
    inputs = glob.glob(PYTHON_TESTS_DIR + '*.py')
    real_inputs = [path for path in inputs if not path.endswith(INTERMEDIATE_EXTENSION)]

    print 'Bootstrapping Python tests...'
    print 'Testing using the following %d input file(s):' % len(real_inputs)

    # Bring blank line.
    print

    for path in real_inputs:
        info = path

        args = code_transformer_cmdl_args(path)
        if options.debug_mode:
            print subprocess.list2cmdline(args)

        retcode = subprocess.call(args)

        if retcode != 0:
            info += (colors.WARNING + ' [FAILED]' + colors.ENDC)
            print info
            continue

        args = compiler_cmdl_args(path)
        if options.debug_mode:
            print subprocess.list2cmdline(args)

        retcode = subprocess.call(args)
        if retcode != 0:
            info += (colors.WARNING + ' [ERROR]' + colors.ENDC)
            print info
            continue

        args = corevm_cmdl_args(path)
        if options.debug_mode:
            print subprocess.list2cmdline(args)

        retcode = StdoutComparator(args, python_cmdl_args(path)).run()

        if retcode == 0:
            info += (colors.OKGREEN + ' [SUCCESS]' + colors.ENDC)
        else:
            info += (colors.FAIL + ' [FAILED]' + colors.ENDC)

        print info

    if not options.debug_mode:
        outputs = glob.glob(PYTHON_TESTS_DIR + '*.tmp.py')
        for output in outputs:
            os.remove(output)

        outputs = glob.glob(PYTHON_TESTS_DIR + '*.core')
        for output in outputs:
            os.remove(output)


def main():
    parser = optparse.OptionParser(
        usage='usage: %prog [options]',
        version='%prog v0.1')

    parser.add_option(
        '-d',
        '--debug',
        action='store_true',
        dest='debug_mode',
        help='Debug mode'
    )

    options, _ = parser.parse_args()

    run(options)


if __name__ == '__main__':
    main()
