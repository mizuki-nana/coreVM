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

import optparse
import os
import subprocess
import sys

from colors import colors

## -----------------------------------------------------------------------------

PYTHON = 'python'
PYTHON_COMPILER = './python/python_compiler.py'
PYTHON_CODE_TRANSFORMER = './python/code_transformer.py'
METADATA_FILE = './corevm_metadata.json'
COREVM = './coreVM'
INTERMEDIATE_EXTENSION = '.tmp.py'
BYTECODE_EXTENSION = '.core'

## -----------------------------------------------------------------------------

class PytaStep(object):

    def __init__(self, cmdl_args, is_severe=False):
        self.cmdl_args = cmdl_args
        self.is_severe = is_severe

    def run(self):
        retcode = subprocess.call(self.cmdl_args)
        if retcode != 0:
            status = '{color}{status}'.format(
                color=colors.WARNING if not self.is_severe else colors.FAIL,
                status='ERROR' if not self.is_severe else 'FAIL')
        else:
            status = '{color}{status}'.format(
                color=colors.OKGREEN,
                status='SUCCESS')

        status += colors.ENDC

        return retcode, status

## -----------------------------------------------------------------------------

class Pyta(object):

    def __init__(self, input_path, debug_mode=False):
        self.input_path = input_path
        self.debug_mode = debug_mode

        self.init_steps()

    def init_steps(self):
        code_transformed_path = \
            self.code_transformer_input_to_output_path(self.input_path)
        compiled_path = \
            self.compiler_input_to_output_path(code_transformed_path)

        self.steps = (
            PytaStep(
                [
                    PYTHON,
                    PYTHON_CODE_TRANSFORMER,
                    '--input',
                    self.input_path,
                    '--output',
                    code_transformed_path
                ]
            ),
            PytaStep(
                [
                    PYTHON,
                    PYTHON_COMPILER,
                    '--input',
                    code_transformed_path,
                    '--metadata-file',
                    METADATA_FILE,
                    '--output',
                    compiled_path
                ]
            ),
            PytaStep(
                [
                    COREVM,
                    compiled_path
                ],
                is_severe=True
            ),
        )

    def code_transformer_input_to_output_path(self, path):
        return os.path.splitext(path)[0] + INTERMEDIATE_EXTENSION

    def compiler_input_to_output_path(self, path):
        return path + BYTECODE_EXTENSION

    def run(self):
        for step in self.steps:
            if self.debug_mode:
                print subprocess.list2cmdline(step.cmdl_args)

            retcode, status = step.run()

            if self.debug_mode or retcode != 0:
                print status + '\n'

            if retcode != 0:
                break

## -----------------------------------------------------------------------------

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

    options, args = parser.parse_args()

    if not args:
        sys.stderr.write('Input file not specified\n')
        sys.exit(-1)

    pyta = Pyta(args[0], debug_mode=options.debug_mode)
    pyta.run()

## -----------------------------------------------------------------------------

if __name__ == '__main__':
    main()

## -----------------------------------------------------------------------------
