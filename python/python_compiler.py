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
import traceback

from datetime import datetime


INSTR_STR_TO_CODE_MAP = 'INSTR_STR_TO_CODE_MAP'
DYOBJ_FLAG_STR_TO_VALUE_MAP = 'DYOBJ_FLAG_STR_TO_VALUE_MAP'


class Instr(object):

    def __init__(self, code, oprd1, oprd2):
        self.code = code
        self.oprd1 = oprd1
        self.oprd2 = oprd2

    def to_json(self):
        return [self.code, self.oprd1, self.oprd2]


class Closure(object):

    # Has to be a non-zero value.
    __closure_id = 1

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

    def __init__(self, options):
        self.output_file = options.output_file
        self.debug_mode = options.debug_mode

        # Read info file
        with open(options.info_file, 'r') as fd:
            info_json = simplejson.load(fd)

        self.instr_str_to_code_map = info_json[INSTR_STR_TO_CODE_MAP]
        self.dyobj_flag_str_to_value_map = info_json[DYOBJ_FLAG_STR_TO_VALUE_MAP]

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
        # TDOO: [COREVM-177] Add support for name mingling in Python compiler
        return name

    def __get_encoding_id(self, name):
        if name not in self.encoding_map:
            self.encoding_id += 1
            self.encoding_map[name] = self.encoding_id

        return self.encoding_map[name]

    def __get_dyobj_flag(self, flags):
        value = 0

        for flag in flags:
            value |= (1 << self.dyobj_flag_str_to_value_map[flag])

        return value

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

        # Off-load arguments.
        self.visit(node.args)

        # Statements.
        for stmt in node.body:
            self.visit(stmt)

        # Explicit return.
        self.__add_instr('rtrn', 0, 0)

        # step out
        self.current_closure_name = self.closure_map[self.current_closure_name].parent_name

        # In the outer closure, set the closure id on the object

        self.__add_instr('new', self.__get_dyobj_flag(['DYOBJ_IS_NOT_GARBAGE_COLLECTIBLE']), 0)
        self.__add_instr('setctx', self.closure_map[name].closure_id, 0)
        self.__add_instr('stobj', self.__get_encoding_id(node.name), 0)

    def visit_Return(self, node):
        # TODO: [COREVM-176] Support return value in Python
        self.__add_instr('rtrn', 0, 0)

    def visit_Print(self, node):
        # TODO: [COREVM-178] Support for printing multiple values in Python
        if node.values:
            self.visit(node.values[0])

        self.__add_instr('print', 0, 0)

    def visit_Expr(self, node):
        self.visit(node.value)

    """ ----------------------------- expr --------------------------------- """

    def visit_BinOp(self, node):
        self.visit(node.right)
        self.visit(node.left)
        self.visit(node.op)

    def visit_Call(self, node):
        # explicit args
        for arg in node.args:
            self.visit(arg)

        # explicit kwargs
        for keyword in node.keywords:
            self.visit(keyword.value)

        # implicit args
        if node.starargs:
            self.visit(node.starargs)

        # implicit kwargs
        if node.kwargs:
            self.visit(node.kwargs)

        self.visit(node.func)
        self.__add_instr('pinvk', 0, 0)

        # The order of loading arguments onto the next frame has to be opposite
        # than the way they are being evaluated, since they are placed on the
        # stack.

        # implicit kwargs
        if node.kwargs:
            self.__add_instr('putkwargs', 0, 0)

        # implicit args
        if node.starargs:
            self.__add_instr('putargs', 0, 0)

        # explicit kwargs
        for keyword in node.keywords:
            self.__add_instr('putkwarg', self.__get_encoding_id(keyword.arg), 0)

        # explicit args
        for arg in node.args:
            self.__add_instr('putarg', 0, 0)

        self.__add_instr('invk', 0, 0)

    def visit_Num(self, node):
        self.__add_instr('new', 0, 0)
        self.__add_instr('uint32', node.n, 0)
        self.__add_instr('sethndl', 0, 0)

    def visit_Name(self, node):
        name = node.id

        if isinstance(node.ctx, ast.Load):
            self.__add_instr('ldobj', self.__get_encoding_id(name), 0)
        elif isinstance(node.ctx, ast.Param):
            # For loading parameters
            # Note: here we only want to handle args. kwargs are handled
            # differently in `visit_arguments`.
            self.__add_instr('getarg', 0, 0)
            self.__add_instr('stobj', self.__get_encoding_id(node.id), 0)
        else:
            # TODO: Add support for other types of ctx of `Name` node.
            pass

    def visit_Str(self, node):
        self.__add_instr('new', 0, 0)
        self.__add_instr('str', self.__get_encoding_id(node.s), 0)
        self.__add_instr('sethndl', 0, 0)

    """ --------------------------- operator ------------------------------- """

    def __add_binary_operator_instr(self, code):
        # TODO: replace simplistic boxing logic here.
        self.__add_instr('gethndl', 0, 0)
        self.__add_instr('pop', 0, 0)
        self.__add_instr('gethndl', 0, 0)
        self.__add_instr('pop', 0, 0)
        self.__add_instr(code, 0, 0)
        self.__add_instr('new', self.__get_dyobj_flag(['DYOBJ_IS_NOT_GARBAGE_COLLECTIBLE']), 0)
        self.__add_instr('sethndl', 0, 0)

    def visit_Add(self, node):
        self.__add_binary_operator_instr('add')

    def visit_Sub(self, node):
        self.__add_binary_operator_instr('sub')

    def visit_Mult(self, node):
        self.__add_binary_operator_instr('mul')

    def visit_Div(self, node):
        self.__add_binary_operator_instr('div')

    def visit_Mod(self, node):
        self.__add_binary_operator_instr('mod')

    def visit_Pow(self, node):
        self.__add_binary_operator_instr('pow')

    def visit_LShift(self, node):
        self.__add_binary_operator_instr('bls')

    def visit_RShift(self, node):
        self.__add_binary_operator_instr('rls')

    def visit_BitOr(self, node):
        self.__add_binary_operator_instr('bor')

    def visit_BitXor(self, node):
        self.__add_binary_operator_instr('bxor')

    def visit_BitAnd(self, node):
        self.__add_binary_operator_instr('band')

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
        """Visit arguments for the function/lambda definition.

        Python treats args and kwargs the same and put them all into the
        `node.args` attribute, and since we are considering them differently
        here, we need to be able to distinguish them.
        """

        # A mapping of the closest arguments to their default values by their
        # column offset, for explicit kwargs.
        closest_args_to_defaults = {}

        # Traverse through the default values, and find the closest arguments
        # for each of them, and put them into the mapping above.
        for default in node.defaults:
            closest_arg = None
            for arg in node.args:
                if closest_arg is None:
                    closest_arg = arg
                elif arg.col_offset > closest_arg.col_offset and arg.col_offset < default.col_offset:
                    closest_arg = arg

            assert closest_arg

            closest_args_to_defaults[closest_arg.col_offset] = default

        # Iterate through the arguments.
        for arg in node.args:
            default = closest_args_to_defaults.get(arg.col_offset)
            if default:
                # This is a kwarg. It needs special handling here.
                #
                # TODO|HACK: This is a hack here to get around of the problem of
                # evaluating a stmt if the kwarg cannot be found on the frame.
                # The condition is that if it's not found, jump over two instrs
                # to execute that stmt, otherwise, jump to skip over that block
                # of code.
                #
                # The problem here is that we don't know the number of instrs to
                # jump over before hand, so we will need to calculate the
                # difference in the size of the vector, and reset the `jmp`
                # instr afterward.
                self.__add_instr('getkwarg', self.__get_encoding_id(arg.id), 2)
                self.__add_instr('jmp', 0, 0) # offset addr to be set
                vector_length1 = len(self.__current_vector())
                self.visit(default)
                vector_length2 = len(self.__current_vector())
                length_diff = vector_length2 - vector_length1
                self.__current_vector()[vector_length1 - 1] = Instr(
                    self.instr_str_to_code_map['jmp'], length_diff + 1, 0)
            else:
                # This is an arg. It's handled in `visit_Name()`.
                self.visit(arg)

        # Pull out rest of the args (*args).
        if node.vararg:
            self.__add_instr('getargs', 0, 0)
            # TODO: Retrieve args stored as an array on top of eval stack.

        # Pull out rest of the kwargs (**kwarg).
        if node.kwarg:
            self.__add_instr('getkwargs', 0, 0)
            # TODO: retrieve kwargs stored as a map on top of eval stack.


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
        '-f',
        '--info-file',
        action='store',
        dest='info_file',
        help='Info File')

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

    if not options.info_file:
        sys.stderr.write('Info file not specified\n')
        return -1

    if not options.output_file:
        sys.stderr.write('Output file not specified\n')
        return -1

    try:
        generator = BytecodeGenerator(options)

        with open(options.input_file, 'r') as fd:
            tree = ast.parse(fd.read())

        generator.visit(tree)
        generator.finalize()
    except Exception as ex:
        sys.stderr.write('Failed to compile %s\n' % options.input_file)
        sys.stderr.write(str(ex))
        sys.stderr.write('\n')
        if options.debug_mode:
            print traceback.format_exc()
        sys.exit(-1)


if __name__ == '__main__':
    main()
