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
import pprint
import simplejson
import sys

from datetime import datetime


class Instr(object):

    def __init__(self, code, oprd1, oprd2):
        self.code = code
        self.oprd1 = oprd1
        self.oprd2 = oprd2

    def to_json(self):
        return [self.code, self.oprd1, self.oprd2]


class Closure(object):

    __closure_id = 0

    def __init__(self, name, parent_name, parent_id):
        self.name = name
        self.parent_name = parent_name
        self.vector = []
        self.closure_id = Closure.__closure_id
        self.parent_id = parent_id
        Closure.__closure_id += 1

    def to_json(self):
        json_dict = {
            '__id__': self.closure_id,
            '__vector__': [
                instr.to_json() for instr in self.vector
            ]
        }

        if self.parent_id is not None:
            json_dict['__parent__'] = self.parent_id

        return json_dict


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

    default_closure_name = '__main__'

    def __init__(self, output_file, instr_str_to_code_map, debug_mode=False):
        self.output_file = output_file
        self.instr_str_to_code_map = instr_str_to_code_map
        self.debug_mode = debug_mode

        # encoding map
        self.encoding_id = 0
        self.encoding_map = dict()

        # closure map
        self.current_closure_name = self.default_closure_name
        self.closure_map = {
            self.current_closure_name: Closure(self.current_closure_name, '', None)
        }

    def finalize(self):
        structured_bytecode = {
            'format': self.format,
            'format-version': self.format_version,
            'target-version': self.target_version,
            'path': '',
            'timestamp': datetime.now().strftime('%Y-%m-%dT%H:%M:%S'),
            'encoding': self.encoding,
            'author': self.author,
            'encoding_map': [
                {
                    'key': key,
                    'value': value
                }
                for key, value in self.encoding_map.iteritems()
            ],
            '__MAIN__': [
                closure.to_json() for closure in self.closure_map.itervalues()
            ]
        }

        if self.debug_mode:
            print 'Writing the following to %s' % self.output_file
            pprint.pprint(structured_bytecode)

        with open(self.output_file, 'w') as fd:
            fd.write(simplejson.dumps(structured_bytecode))

    def __current_vector(self):
        return self.closure_map[self.current_closure_name].vector

    def __add_instr(self, code, oprd1, oprd2):
        self.__current_vector().append(
            Instr(
                self.instr_str_to_code_map[code],
                oprd1,
                oprd2
            )
        )

    def __mingle_name(self, name):
        # TDOO: Add support for actual name minglings.
        return name

    def __get_encoding_id(self, name):
        if name not in self.encoding_map:
            self.encoding_id += 1
            self.encoding_map[name] = self.encoding_id

        return self.encoding_map[name]

    """ ----------------------------- stmt --------------------------------- """

    def visit_FunctionDef(self, node):
        # step in
        name = self.__mingle_name(node.name)

        self.closure_map[name] = Closure(
            name,
            self.current_closure_name,
            self.closure_map[self.current_closure_name].closure_id
        )

        self.current_closure_name = name

        for stmt in node.body:
            self.visit(stmt)
        self.visit(node.args)

        # step out
        self.current_closure_name = self.closure_map[self.current_closure_name].parent_name

        # In the outer closure, set the closure id on the object

        # TODO: [COREVM-170] Add support for object creation flags in Python compiler
        self.__add_instr('new', 0, 0)
        self.__add_instr('setctx', self.closure_map[name].closure_id, 0)
        self.__add_instr('stobj', self.__get_encoding_id(node.name), 0)

    def visit_Expr(self, node):
        self.visit(node.value)

    """ ----------------------------- expr --------------------------------- """

    def visit_BinOp(self, node):
        self.visit(node.right)
        self.visit(node.left)
        self.visit(node.op)

    def visit_Call(self, node):
        # TODO: [COREVM-169] Add support for functional call arguments in Python tests
        self.visit(node.func)
        self.__add_instr('pinvk', 0, 0)
        self.__add_instr('invk', 0, 0)

    def visit_Num(self, node):
        self.__add_instr('uint32', node.n, 0)

    def visit_Name(self, node):
        name = node.id

        if isinstance(node.ctx, ast.Load):
            self.__add_instr('ldobj', self.__get_encoding_id(name), 0)
        else:
            # TODO: Add support for other types of ctx of `Name` node.
            pass

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

    """ --------------------------- arguments ------------------------------ """

    def visit_arguments(self, node):
        # TODO: [COREVM-169] Add support for functional call arguments in Python tests
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

    parser.add_option(
        '-d',
        '--debug',
        action='store_true',
        dest='debug_mode',
        help='Debug mode'
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

    generator = BytecodeGenerator(
        options.output_file,
        instr_str_to_code_map,
        debug_mode=options.debug_mode
    )

    try:
        generator.visit(tree)
        generator.finalize()
    except Exception as ex:
        sys.stderr.write('Failed to compile %s\n' % options.input_file)
        sys.stderr.write(str(ex))
        sys.stderr.write('\n')
        sys.exit(-1)


if __name__ == '__main__':
    main()
