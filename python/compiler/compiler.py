# The MIT License (MIT)

# Copyright (c) 2016 Yanzheng Li

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

# Program entry point of Python compiler.

import optparse
import os
import subprocess
import sys


## -----------------------------------------------------------------------------

class colors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

## -----------------------------------------------------------------------------

class Compiler(object):

    VERSION = 'v0.1'
    PYTHON = 'python'
    CODEGEN = './python/compiler/codegen.py'
    CODE_TRANSFORMER = './python/compiler/code_transformer.py'
    METADATA_FILE = './build/artifacts/corevm_metadata.json'
    INTERMEDIATE_EXTENSION = '.tmp.py'
    BYTECODE_EXTENSION = '.core'

    class CompilerStep(object):

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

    def __init__(self, input_path, opts):
        self.input_path = input_path
        self.opts = opts

        self.init_steps()

    def init_steps(self):
        code_transformed_path = \
            self.code_transformer_input_to_output_path(self.input_path)
        compiled_path = \
            self.compiler_input_to_output_path(code_transformed_path)

        self.steps = [
            self.CompilerStep(
                [
                    self.PYTHON,
                    self.CODE_TRANSFORMER,
                    '--input',
                    self.input_path,
                    '--output',
                    code_transformed_path
                ]
            ),
            self.CompilerStep(
                [
                    self.PYTHON,
                    self.CODEGEN,
                    '--input',
                    code_transformed_path,
                    '--metadata-file',
                    self.METADATA_FILE,
                    '--output',
                    compiled_path
                ]
            ),
        ]

    @classmethod
    def code_transformer_input_to_output_path(self, path):
        return os.path.splitext(path)[0] + self.INTERMEDIATE_EXTENSION

    @classmethod
    def compiler_input_to_output_path(self, path):
        return path + self.BYTECODE_EXTENSION

    @classmethod
    def input_to_output_path(cls, path):
        return cls.compiler_input_to_output_path(
            cls.code_transformer_input_to_output_path(path))

    def run(self):
        retcode = 0

        for step in self.steps:
            if self.opts.verbose:
                print subprocess.list2cmdline(step.cmdl_args)

            retcode, status = step.run()

            if self.opts.verbose or retcode != 0:
                print status + '\n'

            if retcode != 0:
                break

        return retcode

## -----------------------------------------------------------------------------

def main():
    parser = optparse.OptionParser(
        description='Python compiler {version}'.format(version=Compiler.VERSION),
        usage='usage: %prog [options]',
        version='%prog {version}'.format(version=Compiler.VERSION))

    parser.add_option(
        '-v',
        '--verbose',
        action='store_true',
        dest='verbose',
        help='Verbose mode')

    options, args = parser.parse_args()

    if not args:
        sys.stderr.write('Input file not specified\n')
        sys.exit(-1)

    retcode = Compiler(args[0], options).run()

    sys.exit(retcode)

## -----------------------------------------------------------------------------

if __name__ == '__main__':
    main()
