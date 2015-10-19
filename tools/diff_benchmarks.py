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
import simplejson
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

def percent_diff(old_val, new_val):
    return abs(new_val - old_val) / float(old_val) * 100.0

## -----------------------------------------------------------------------------

class Difference(object):

    pprint_formatter = '{NAME_COLOR_BEGIN}{name}{NAME_COLOR_END} - '\
            '{OLD_VALUE_COLOR_BEGIN}{old_value}{OLD_VALUE_COLOR_END} vs. '\
            '{NEW_VALUE_COLOR_BEGIN}{new_value}{NEW_VALUE_COLOR_END}{unit} '\
            '({PERCENTAGE_COLOR_BEGIN}{percentage:.2f}%{PERCENTAGE_COLOR_END} '\
            '{gain_or_loss})'

    def __init__(self, name, unit, left_value, right_value):
        self.name = name
        self.unit = unit
        self.left_value = left_value
        self.right_value = right_value

    def pprint(self, is_gain=True, show_color=True):
        NAME_COLOR = colors.OKBLUE if show_color else colors.ENDC
        OLD_VALUE_COLOR = (colors.FAIL if is_gain else colors.OKGREEN) if show_color else colors.ENDC
        NEW_VALUE_COLOR = (colors.OKGREEN if is_gain else colors.FAIL) if show_color else colors.ENDC
        PERCENTAGE_COLOR = colors.WARNING if show_color else colors.ENDC
        gain_or_loss = 'improvement' if is_gain else 'loss'

        print self.pprint_formatter.format(
            NAME_COLOR_BEGIN=NAME_COLOR,
            name=self.name,
            NAME_COLOR_END=colors.ENDC,

            OLD_VALUE_COLOR_BEGIN=OLD_VALUE_COLOR,
            old_value=self.left_value,
            OLD_VALUE_COLOR_END=colors.ENDC,

            NEW_VALUE_COLOR_BEGIN=NEW_VALUE_COLOR,
            new_value=self.right_value,
            NEW_VALUE_COLOR_END=colors.ENDC,

            unit=self.unit,

            PERCENTAGE_COLOR_BEGIN=PERCENTAGE_COLOR,
            percentage=percent_diff(self.left_value, self.right_value),
            PERCENTAGE_COLOR_END=colors.ENDC,

            gain_or_loss=gain_or_loss)

## -----------------------------------------------------------------------------

class BenchmarkDiffer(object):

    REAL_TIME_METRIC = 'real_time'
    CPU_TIME_METRIC = 'cpu_time'
    DEFAULT_COMPARISON_METRIC = REAL_TIME_METRIC
    SUPPORTED_METRICS = (REAL_TIME_METRIC, CPU_TIME_METRIC,)

    def __init__(self, left_path, right_path, opts):
        self.left_path = left_path
        self.right_path = right_path
        self.opts = opts
        self.left_content = None
        self.right_content = None

        self.init()

    def init(self):
        with open(self.left_path) as fd:
            self.left_content = simplejson.load(fd)

        with open(self.right_path) as fd:
            self.right_content = simplejson.load(fd)

    def __print_path(self, path):
        formatter = '{color}{path}{end_color}'
        print formatter.format(
            color=colors.OKBLUE if self.opts.show_color else colors.ENDC,
            path=path, end_color=colors.ENDC)

    def __print_metric(self):
        formatter = 'Metric: {color}{metric}{end_color}'
        print formatter.format(
            color=colors.OKBLUE if self.opts.show_color else colors.ENDC,
            metric=self.opts.comparison_metric,
            end_color=colors.ENDC)

    def __print_diffs(self, diffs, title, is_gain):
        title_color = colors.HEADER if self.opts.show_color else colors.ENDC
        print '{title_color}{diff_count} {title}{end_color}'.format(
            title_color=title_color,
            diff_count=len(diffs),
            title=title,
            end_color=colors.ENDC)
        for item in diffs:
            item.pprint(is_gain=is_gain, show_color=self.opts.show_color)

    def run_diff(self):
        assert 'benchmarks' in self.left_content
        assert 'benchmarks' in self.right_content

        improvements = []
        losses = []

        left_benchmarks_by_name = dict(
            (benchmark['name'], benchmark)
            for benchmark in self.left_content['benchmarks'])

        right_benchmarks_by_name = dict(
            (benchmark['name'], benchmark)
            for benchmark in self.right_content['benchmarks'])

        for left_key, left_value in left_benchmarks_by_name.iteritems():
            if left_key in right_benchmarks_by_name:
                right_value = right_benchmarks_by_name[left_key]

                left_time = left_value.get(self.opts.comparison_metric)
                right_time = right_value.get(self.opts.comparison_metric)

                assert left_time is not None
                assert right_time is not None

                DIFF_UNIT = 's'
                if left_time > right_time:
                    improvements.append(Difference(left_key, DIFF_UNIT, left_time, right_time))
                elif left_time < right_time:
                    losses.append(Difference(left_key, DIFF_UNIT, left_time, right_time))

        print 'Diffing between the following two files:'
        self.__print_path(self.left_path)
        self.__print_path(self.right_path)
        self.__print_metric()
        print '\n'

        if not improvements and not losses:
            print 'No diffs'
            return

        if improvements:
            self.__print_diffs(improvements, 'Improvements', True)

        print '\n'

        if losses:
            self.__print_diffs(losses, 'Losses', False)

## -----------------------------------------------------------------------------

def main():
    parser = optparse.OptionParser(
        usage='usage: %prog LEFT_FILE RIGHT_FILE [options]',
        version='%prog v1.0')

    parser.add_option(
        '-c',
        '--color',
        action='store_true',
        dest='show_color',
        help='Display outputs in colors')

    parser.add_option(
        '-m',
        '--metric',
        action='store',
        type='string',
        dest='comparison_metric',
        default=BenchmarkDiffer.DEFAULT_COMPARISON_METRIC,
        help='Comparion metric=<%s>' % '|'.join(BenchmarkDiffer.SUPPORTED_METRICS))

    opts, args = parser.parse_args()

    if len(args) < 2:
        sys.stderr.write('Invalid arguments\n')
        sys.exit(-1)

    if opts.comparison_metric not in BenchmarkDiffer.SUPPORTED_METRICS:
        sys.stderr.write('Invalid arguments\n')
        sys.exit(-1)

    left_path = args[0]
    right_path = args[1]

    BenchmarkDiffer(left_path, right_path, opts).run_diff()

## -----------------------------------------------------------------------------

if __name__ == '__main__':
    main()

## -----------------------------------------------------------------------------
