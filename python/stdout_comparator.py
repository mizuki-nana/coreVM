import subprocess

from colors import colors


class StdoutComparator(object):

    def __init__(self, lhs_args, rhs_args):
        self.lhs_args = lhs_args
        self.rhs_args = rhs_args

    def run(self):
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

        output = colors.HEADER + 'Difference in output:\n'
        output += colors.FAIL
        output += lhs_output
        output += '\n'
        output += colors.OKBLUE
        output += rhs_output
        output += colors.ENDC
        output += '\n'

        print output

        return -1
