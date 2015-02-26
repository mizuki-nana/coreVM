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

import ast
import optparse
import simplejson
import sys

from datetime import datetime


class Instr(object):

    def __init__(self, code, oprd1, oprd2):
        self.code = code
        self.oprd1 = oprd1
        self.oprd2 = oprd2


class BytecodeGenerator(ast.NodeVisitor):
    """Traverses through Python AST and generates version and format specific
    coreVM bytecode.

    The traversal follows the abstract grammar of Python, defined here:
    https://docs.python.org/2/library/ast.html#abstract-grammar

    The visit_XXX methods defined below follow the same order in the grammar
    spec.
    """

    format = 'application/json'
    format_version = '0.1'
    target_version = '0.1'
    encoding = 'utf8'
    author = 'Yanzheng Li'

    def __init__(self, output_file, instr_str_to_code_map):
        self.output_file = output_file
        self.instr_str_to_code_map = instr_str_to_code_map
        self.vector = []

    def finalize(self):
        structured_bytecode = {
            'format': self.format,
            'format-version': self.format_version,
            'target-version': self.target_version,
            'path': '',
            'timestamp': datetime.now().strftime('%Y-%m-%dT%H:%M:%S'),
            'encoding': self.encoding,
            'author': self.author,
            'encoding_map': [],
            '__MAIN__': [
                {
                    '__name__': '__main__',
                    '__vector__': [
                        [
                            instr.code,
                            instr.oprd1,
                            instr.oprd2
                        ] for instr in self.vector
                    ]
                },
            ]
        }

        with open(self.output_file, 'w') as fd:
            fd.write(simplejson.dumps(structured_bytecode))

    def __add_instr(self, code, oprd1, oprd2):
        self.vector.append(
            Instr(
                self.instr_str_to_code_map[code],
                oprd1,
                oprd2
            )
        )

    """ ----------------------------- expr --------------------------------- """

    def visit_BinOp(self, node):
        self.visit(node.left)
        self.visit(node.right)
        self.visit(node.op)

    def visit_Num(self, node):
        self.__add_instr('uint32', node.n, 0)

    """ --------------------------- operator ------------------------------- """

    def visit_Add(self, node):
        self.__add_instr('add', 0, 0)

    def visit_Sub(self, node):
        self.__add_instr('sub', 0, 0)

    def visit_Mult(self, node):
        self.__add_instr('mul', 0, 0)

    def visit_Div(self, node):
        self.__add_instr('div', 0, 0)

    def visit_Mod(self, node):
        self.__add_instr('mod', 0, 0)

    def visit_Pow(self, node):
        self.__add_instr('pow', 0, 0)

    def visit_LShift(self, node):
        self.__add_instr('bls', 0, 0)

    def visit_RShift(self, node):
        self.__add_instr('rls', 0, 0)

    def visit_BitOr(self, node):
        self.__add_instr('bor', 0, 0)

    def visit_BitXor(self, node):
        self.__add_instr('bxor', 0, 0)

    def visit_BitAnd(self, node):
        self.__add_instr('band', 0, 0)

    """ ---------------------------- unaryop ------------------------------- """

    def visit_Invert(self, node):
        self.__add_instr('bnot', 0, 0)

    def visit_Not(self, node):
        self.__add_instr('lnot', 0, 0)

    def visit_UAdd(self, node):
        self.__add_instr('pos', 0, 0)

    def visit_USub(self, node):
        self.__add_instr('neg', 0, 0)

    """ ----------------------------- cmpop -------------------------------- """

    def visit_Eq(self, node):
        self.__add_instr('eq', 0, 0)

    def visit_NotEq(self, node):
        self.__add_instr('neq', 0, 0)

    def visit_Lt(self, node):
        self.__add_instr('lt', 0, 0)

    def visit_LtE(self, node):
        self.__add_instr('lte', 0, 0)

    def visit_Gt(self, node):
        self.__add_instr('gt', 0, 0)

    def visit_GtE(self, node):
        self.__add_instr('gte', 0, 0)

    def visit_Is(self, node):
        pass

    def visit_IsNot(self, node):
        pass

    def visit_In(self, node):
        pass

    def visit_NotIn(self, node):
        pass


def main():
    parser = optparse.OptionParser(
        usage='usage: %prog [options]',
        version='%prog v0.1')

    parser.add_option(
        '-i',
        '--input',
        action='store',
        dest='input_file',
        help='Input file'
    )

    parser.add_option(
        '-n',
        '--instr-info-file',
        action='store',
        dest='instr_info_file',
        help='Instruction Info File')

    parser.add_option(
        '-o',
        '--output',
        action='store',
        dest='output_file',
        help='Output file'
    )

    options, _ = parser.parse_args()

    if not options.input_file:
        sys.stderr.write('Input file not specified\n')
        return -1

    if not options.instr_info_file:
        sys.stderr.write('Instruction info file not specified\n')
        return -1

    if not options.output_file:
        sys.stderr.write('Output file not specified\n')
        return -1

    # Extract instr info
    instr_str_to_code_map = None
    with open(options.instr_info_file, 'r') as fd:
        instr_str_to_code_map = simplejson.load(fd)

    with open(options.input_file, 'r') as fd:
        tree = ast.parse(fd.read())

    try:
        generator = BytecodeGenerator(options.output_file, instr_str_to_code_map)
        generator.visit(tree)
        generator.finalize()
    except Exception as ex:
        sys.stderr.write('Failed to compile %s' % options.input_file)
        sys.stderr.write(ex)
        sys.exit(-1)


if __name__ == '__main__':
    main()
