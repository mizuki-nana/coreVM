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

import optparse
import os
import subprocess
import sys

from colors import colors
from compiler import compiler

## -----------------------------------------------------------------------------

PYTHON = 'python'
PYTHON_COMPILER = './python/compiler/compiler.py'
COREVM = './build/src/coreVM'
COREVM_DBG = './build/src/coreVM_dbg'
COREVM_DBG_ASAN = './build/src/coreVM_dbg_asan'

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

    def __init__(self, input_path, options):
        self.input_path = input_path
        self.options = options

        self.init_steps()

    def init_steps(self):
        compiled_path = \
            compiler.Compiler.input_to_output_path(self.input_path)

        compilation_step = PytaStep(
            [
                PYTHON,
                PYTHON_COMPILER,
                self.input_path,
            ])

        if self.options.verbose:
            compilation_step.cmdl_args.append('--verbose')

        run_step = None

        if self.options.sanity_test:
            run_step = PytaStep(
                    [
                        COREVM_DBG_ASAN,
                        compiled_path
                    ],
                    is_severe=True)
        elif self.options.dynamic_analysis:
            # Run coreVM with Valgrind's Memcheck.
            run_step = PytaStep(
                    [
                        'valgrind',
                        '--tool=memcheck',
                        '--gen-suppressions=no',

                        # Do not print detailed leak analysis.
                        # (May want to turn it on for individual tests).
                        '--leak-check=no',

                        # Show "still reachable" leaks.
                        # http://valgrind.org/docs/manual/faq.html#faq.deflost
                        '--show-reachable=yes',

                        # Do not generate mingled names.
                        '--demangle=yes',

                        # A non-zero exit code to indicate unsuppressed errors
                        # reported.
                        '--error-exitcode=20',

                        # Supply a list of suppressions.
                        '--suppressions=valgrind/memcheck.supp',

                        # Only print errors.
                        '--quiet',

                        COREVM_DBG,
                        compiled_path
                    ],
                    is_severe=True)
        else:
            run_step = PytaStep(
                    [
                        COREVM,
                        compiled_path
                    ],
                    is_severe=True)

        if self.options.args:
            run_step.cmdl_args.append(self.options.args)

        self.steps = (compilation_step, run_step)

    def run(self):
        for step in self.steps:
            if self.options.verbose:
                print subprocess.list2cmdline(step.cmdl_args)

            retcode, status = step.run()

            if self.options.verbose or retcode != 0:
                print status + '\n'

            if retcode != 0:
                break

## -----------------------------------------------------------------------------

def main():
    parser = optparse.OptionParser(
        usage='usage: %prog [options]',
        version='%prog v0.1')

    parser.add_option(
        '-v',
        '--verbose',
        action='store_true',
        dest='verbose',
        help='Verbose mode'
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
        '-d',
        '--dynamic-analysis',
        action='store_true',
        dest='dynamic_analysis',
        default=False,
        help='Enable dynamic analysis'
    )

    parser.add_option(
        '-a',
        '--args',
        action='store',
        dest='args',
        default='',
        help='Arguments passed to coreVM'
    )

    options, args = parser.parse_args()

    if not args:
        sys.stderr.write('Input file not specified\n')
        sys.exit(-1)

    pyta = Pyta(args[0], options)
    pyta.run()

## -----------------------------------------------------------------------------

if __name__ == '__main__':
    main()

## -----------------------------------------------------------------------------
