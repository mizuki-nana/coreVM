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
import itertools
import optparse
import pprint
import random
import simplejson
import string
import sys
import traceback

from datetime import datetime


INSTR_STR_TO_CODE_MAP = 'INSTR_STR_TO_CODE_MAP'
DYOBJ_FLAG_STR_TO_VALUE_MAP = 'DYOBJ_FLAG_STR_TO_VALUE_MAP'


class VectorString(object):

    def __init__(self, s):
        self.s = s

    @classmethod
    def is_vector_string(cls, s):
        # TODO: Need more robust checking.
        s = s.strip()
        return s.startswith('### BEGIN VECTOR ###') and s.endswith('### END VECTOR ###')

    def to_raw_vector(self):
        vector = []

        lines = self.s.split('\n')

        for line in lines:
            if not 'VECTOR' in line and len(line) > 5:
                line = line.strip('[ ]\n')
                vector.append(line.split(','))

        return vector


class Instr(object):

    def __init__(self, code, oprd1, oprd2):
        self.code = code
        self.oprd1 = oprd1
        self.oprd2 = oprd2

    def to_json(self):
        return [self.code, self.oprd1, self.oprd2]


class Loc(object):

    def __init__(self, lineno, col_offset):
        self.lineno = lineno
        self.col_offset = col_offset

    @classmethod
    def from_node(cls, node):
        return Loc(node.lineno, node.col_offset)


class Closure(object):

    # Has to be a non-zero value.
    __closure_id = 1

    def __init__(self, original_name, name, parent_name, parent_id):
        self.original_name = original_name
        self.name = name
        self.parent_name = parent_name
        self.vector = []
        self.locs = {} # mapping of instr index to loc info
        self.closure_id = Closure.__closure_id
        self.parent_id = parent_id
        Closure.__closure_id += 1

    def add_loc(self, index, loc):
        self.locs[index] = loc

    def to_json(self):
        json_dict = {
            'name': self.original_name,
            '__id__': self.closure_id,
            '__vector__': [
                instr.to_json() for instr in self.vector
            ]
        }

        if self.parent_id is not None:
            json_dict['__parent__'] = self.parent_id

        if self.locs:
            json_dict['locs'] = [
                {
                    'index': index,
                    'loc': {
                        'lineno': loc.lineno,
                        'col_offset': loc.col_offset
                    }
                }
                for index, loc in self.locs.iteritems()
            ]

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
        self.input_file = options.input_file
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
            self.current_closure_name: Closure(
                self.current_closure_name, self.current_closure_name, '', None)
        }

        # states
        self.current_class_name = ''

    def read_from_source(self, path):
        with open(path, 'r') as fd:
            tree = ast.parse(fd.read())

        self.visit(tree)

    def finalize(self):
        structured_bytecode = {
            'format': self.format,
            'format-version': self.format_version,
            'target-version': self.target_version,
            'path': self.input_file,
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

    def __current_closure(self):
        return self.closure_map[self.current_closure_name]

    def __current_vector(self):
        return self.__current_closure().vector

    def __add_instr(self, code, oprd1, oprd2, loc=None):
        self.__current_vector().append(
            Instr(
                self.instr_str_to_code_map[code],
                oprd1,
                oprd2
            )
        )

        if loc:
            self.__current_closure().add_loc(len(self.__current_vector()) - 1, loc)

    def __mingle_name(self, name):
        # TDOO: [COREVM-177] Add support for name mingling in Python compiler
        return self.current_class_name + '.' + name

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

    def __process_raw_instr(self, raw_instr):
        INSTRS_NEED_ENCODING_ID = (
            'ldobj',
            'stobj',
            'setattr',
            'getattr',
            'putkwarg',
            'rsetattrs',
            'str'
        )
        raw_code, raw_oprd1, raw_oprd2 = raw_instr

        raw_code = raw_code.strip()
        raw_oprd1 = raw_oprd1.strip()
        raw_oprd2 = raw_oprd2.strip()

        code = raw_code.strip()

        if raw_code in INSTRS_NEED_ENCODING_ID:
            oprd1 = self.__get_encoding_id(raw_oprd1)
        else:
            oprd1 = int(raw_oprd1)
        oprd2 = int(raw_oprd2)

        return code, oprd1, oprd2

    def __get_random_name(self):
        return ''.join(random.choice(string.ascii_letters) for _ in xrange(5))

    """ ----------------------------- stmt --------------------------------- """

    def visit_FunctionDef(self, node):
        # step in
        name = self.__mingle_name(node.name)

        self.closure_map[name] = Closure(
            node.name,
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
        self.__add_instr('ldobj', self.__get_encoding_id('object'), 0)
        self.__add_instr('setattr', self.__get_encoding_id('__class__'), 0)

        if not self.current_class_name:
            self.__add_instr('stobj', self.__get_encoding_id(node.name), 0)

    def visit_ClassDef(self, node):
        # Step in.
        self.current_class_name = self.current_class_name + '::' + node.name

        self.__add_instr('new', self.__get_dyobj_flag(['DYOBJ_IS_NOT_GARBAGE_COLLECTIBLE']), 0)
        self.__add_instr('setctx', 0, 0)
        self.__add_instr('map', 0, 0)
        self.__add_instr('sethndl', 0, 0)
        self.__add_instr('stobj', self.__get_encoding_id(node.name), 0)
        self.__add_instr('ldobj', self.__get_encoding_id(node.name), 0)
        self.__add_instr('ldobj', self.__get_encoding_id('type'), 0)
        self.__add_instr('setattr', self.__get_encoding_id('__class__'), 0)

        for stmt in node.body:
            # TODO|NOTE: currently only supports functions.
            if isinstance(stmt, ast.FunctionDef):
                self.__add_instr('gethndl', 0, 0)
                self.visit(stmt)
                self.__add_instr('ldobj', self.__get_encoding_id('MethodType'), 0)
                self.__add_instr('setattr', self.__get_encoding_id('__class__'), 0)
                self.__add_instr('mapset', self.__get_encoding_id(stmt.name), 0)
                self.__add_instr('setattr', self.__get_encoding_id(stmt.name), 0)
                self.__add_instr('sethndl', 0, 0)
            elif isinstance(stmt, ast.ClassDef):
                tmp_name = self.__get_random_name()
                self.visit(stmt)
                self.__add_instr('stobj2', self.__get_encoding_id(tmp_name), 0)
                self.__add_instr('ldobj', self.__get_encoding_id(node.name), 0)
                self.__add_instr('ldobj2', self.__get_encoding_id(tmp_name), 0)
                self.__add_instr('setattr', self.__get_encoding_id(stmt.name), 0)
            else:
                # TODO: handle other cases.
                pass

        # Step out.
        self.current_class_name = '::'.join(self.current_class_name.split('::')[:-1])

    def visit_Return(self, node):
        # TODO: [COREVM-176] Support return value in Python
        if node.value:
            self.visit(node.value)
        self.__add_instr('rtrn', 0, 0)

    def visit_Assign(self, node):
        res = self.visit(node.value)

        if isinstance(node.value, ast.Lambda):
            assert res # This is the name of the lambda closure.
            self.__add_instr('setctx', self.closure_map[res].closure_id, 0)

        self.visit(node.targets[0])

    def visit_AugAssign(self, node):
        pass

    def visit_Print(self, node):
        # TODO: [COREVM-178] Support for printing multiple values in Python
        if node.values:
            self.visit(node.values[0])

        self.__add_instr('print', 0, 0, loc=Loc.from_node(node))

    def visit_For(self, node):
        # TODO: Handle `orelse` here.
        index_name = self.__get_random_name()
        iter_name = self.__get_random_name()

        # Store index object into a variable.
        self.__add_instr('new', 0, 0)
        self.__add_instr('uint32', 0, 0)
        self.__add_instr('sethndl', 0, 0)
        self.__add_instr('stobj2', self.__get_encoding_id(index_name), 0)

        # Store iter object into a variable.
        self.visit(node.iter)
        self.__add_instr('stobj2', self.__get_encoding_id(iter_name), 0)

        vector_length1 = len(self.__current_vector())

        self.__add_instr('ldobj2', self.__get_encoding_id(iter_name), 0)
        self.__add_instr('gethndl', 0, 0)
        self.__add_instr('arylen', 0, 0)
        self.__add_instr('ldobj2', self.__get_encoding_id(index_name), 0)
        self.__add_instr('gethndl', 0, 0)
        self.__add_instr('gte', 0, 0) # The opposite of `lt` check.

        self.__add_instr('jmpif', 0, 0)
        vector_length2 = len(self.__current_vector())

        # Get item from array, and load into target.
        self.__add_instr('ldobj2', self.__get_encoding_id(iter_name), 0)
        self.__add_instr('gethndl', 0, 0)
        self.__add_instr('ldobj2', self.__get_encoding_id(index_name), 0)
        self.__add_instr('gethndl', 0, 0)
        self.__add_instr('aryat', 0, 0)
        self.__add_instr('getobj', 0, 0)
        self.__add_instr('stobj', self.__get_encoding_id(node.target.id), 0)

        break_line_length = 0

        # Execute body instruction.
        for stmt in node.body:
            if isinstance(stmt, ast.Break):
                self.__add_instr('jmp', 0, 0)
                break_line_length = len(self.__current_vector())
            elif isinstance(stmt, ast.Continue):
                # Do not run the rest of the statements,
                # just run increment index and loop back below.
                break
            else:
                self.visit(stmt)

        # Increment index.
        self.__add_instr('ldobj2', self.__get_encoding_id(index_name), 0)
        self.__add_instr('gethndl', 0, 0)
        self.__add_instr('inc', 0, 0)
        self.__add_instr('sethndl', 0, 0)
        self.__add_instr('stobj2', self.__get_encoding_id(index_name), 0)
        self.__add_instr('jmpr', vector_length1 - 1, 0)

        vector_length3 = len(self.__current_vector())

        length_diff = vector_length3 - vector_length2
        self.__current_vector()[vector_length2 - 1] = Instr(
            self.instr_str_to_code_map['jmpif'], length_diff, 0)

        if break_line_length:
            break_length_diff = vector_length3 - break_line_length
            self.__current_vector()[break_line_length - 1] = Instr(
                self.instr_str_to_code_map['jmp'], break_length_diff, 0)

    def visit_While(self, node):
        # TODO: Handle `orelse` here.
        vector_length1 = len(self.__current_vector())

        # Test expr.
        self.visit(node.test)

        self.__add_instr('gethndl', 0, 0)
        self.__add_instr('truthy', 0, 0)
        self.__add_instr('lnot', 0, 0)
        self.__add_instr('jmpif', 0, 0)

        vector_length2 = len(self.__current_vector())

        break_line_length = 0

        # Execute body instruction.
        for stmt in node.body:
            if isinstance(stmt, ast.Break):
                self.__add_instr('jmp', 0, 0)
                break_line_length = len(self.__current_vector())
            elif isinstance(stmt, ast.Continue):
                self.__add_instr('jmpr', vector_length1 - 1, 0)
            else:
                self.visit(stmt)

        # Jump back to beginning.
        self.__add_instr('jmpr', vector_length1 - 1, 0)

        vector_length3 = len(self.__current_vector())

        length_diff = vector_length3 - vector_length2
        self.__current_vector()[vector_length2 - 1] = Instr(
            self.instr_str_to_code_map['jmpif'], length_diff, 0)

        if break_line_length:
            break_length_diff = vector_length3 - break_line_length
            self.__current_vector()[break_line_length - 1] = Instr(
                self.instr_str_to_code_map['jmp'], break_length_diff, 0)

    def visit_If(self, node):
        self.visit(node.test)
        self.__add_instr('gethndl', 0, 0, loc=Loc.from_node(node))
        self.__add_instr('truthy', 0, 0, loc=Loc.from_node(node))
        self.__add_instr('lnot', 0, 0, loc=Loc.from_node(node))

        # Add `jmpif` here.
        self.__add_instr('jmpif', 0, 0, loc=Loc.from_node(node))
        vector_length1 = len(self.__current_vector())

        for stmt in node.body:
            self.visit(stmt)

        # Add `jmpif` here.
        vector_length2 = len(self.__current_vector())
        length_diff = vector_length2 - vector_length1
        self.__current_vector()[vector_length1 - 1] = Instr(
            self.instr_str_to_code_map['jmpif'], length_diff, 0)

        for stmt in node.orelse:
            self.visit(stmt)

    def visit_Expr(self, node):
        self.visit(node.value)

    def visit_Pass(self, node):
        pass

    """ ----------------------------- expr --------------------------------- """

    def visit_BoolOp(self, node):
        assert len(node.values) >= 2

        left_name = self.__get_random_name()
        right_name = self.__get_random_name()

        assert left_name != right_name

        left_name_id = self.__get_encoding_id(left_name)
        right_name_id = self.__get_encoding_id(right_name)

        assert isinstance(node.op, ast.And) or isinstance(node.op, ast.Or)

        if isinstance(node.op, ast.Or):
            jmp_lengths = []

            self.visit(node.values[0])
            self.__add_instr('stobj2', left_name_id, 0)

            self.visit(node.values[1])
            self.__add_instr('stobj2', right_name_id, 0)

            self.__add_instr('ldobj2', left_name_id, 0)
            self.__add_instr('gethndl', 0, 0)
            self.__add_instr('ldobj2', right_name_id, 0)
            self.__add_instr('gethndl', 0, 0)
            self.__add_instr('lor', 0, 0)
            self.__add_instr('jmpif', 0, 0)

            jmp_lengths.append(len(self.__current_vector()))

            for i in xrange(2, len(node.values)):
                self.visit(node.values[i])

                self.__add_instr('gethndl', 0, 0)
                self.__add_instr('bool', 0, 0)
                self.__add_instr('lor', 0, 0)
                self.__add_instr('jmpif', 0, 0)

                jmp_lengths.append(len(self.__current_vector()))

            current_length = len(self.__current_vector())
            self.__add_instr('cldobj',
                self.__get_encoding_id('True'), self.__get_encoding_id('False'))

            for jmp_length in jmp_lengths:
                length_diff = current_length - jmp_length

                self.__current_vector()[jmp_length - 1] = Instr(
                    self.instr_str_to_code_map['jmpif'], length_diff, 0)
        elif isinstance(node.op, ast.And):
            self.visit(node.values[0])
            self.__add_instr('stobj2', left_name_id, 0)

            self.visit(node.values[1])
            self.__add_instr('stobj2', right_name_id, 0)

            self.__add_instr('ldobj2', left_name_id, 0)
            self.__add_instr('gethndl', 0, 0)
            self.__add_instr('ldobj2', right_name_id, 0)
            self.__add_instr('gethndl', 0, 0)
            self.__add_instr('land', 0, 0)
            self.__add_instr('sethndl', 0, 0) # store in right

            for i in xrange(2, len(node.values)):
                self.visit(node.values[i])
                self.__add_instr('gethndl', 0, 0)
                self.__add_instr('ldobj2', right_name_id, 0)
                self.__add_instr('gethndl', 0, 0)
                self.__add_instr('land', 0, 0)
                self.__add_instr('sethndl', 0, 0) # store in right

            self.__add_instr('ldobj2', right_name_id, 0)
            self.__add_instr('gethndl', 0, 0)
            self.__add_instr('cldobj',
                self.__get_encoding_id('True'), self.__get_encoding_id('False'))

    def visit_BinOp(self, node):
        self.visit(node.right)
        self.visit(node.left)
        self.visit(node.op)

    def visit_Lambda(self, node):
        # This is very similar to `visit_FunctionDef`.

        # step in
        name = 'lambda_' + self.__get_random_name()

        self.closure_map[name] = Closure(
            name,
            name,
            self.current_closure_name,
            self.closure_map[self.current_closure_name].closure_id
        )

        self.current_closure_name = name

        # Off-load arguments.
        self.visit(node.args)

        # Expr.
        self.visit(node.body)

        # Explicit return.
        self.__add_instr('rtrn', 0, 0)

        # step out
        self.current_closure_name = self.closure_map[self.current_closure_name].parent_name

        return name

    def visit_IfExp(self, node):
        self.visit(node.test)

        self.__add_instr('gethndl', 0, 0)
        self.__add_instr('truthy', 0, 0)
        self.__add_instr('jmpif', 0, 0)

        vector_length1 = len(self.__current_vector())

        self.visit(node.orelse)

        vector_length2 = len(self.__current_vector())
        length_diff = vector_length2 - vector_length1

        self.visit(node.body)

        self.__current_vector()[vector_length1 - 1] = Instr(
            self.instr_str_to_code_map['jmpif'], length_diff, 0)

    def visit_Dict(self, node):
        assert len(node.keys) == len(node.values)

        dict_name = self.__get_random_name()

        self.__add_instr('new', 0, 0)
        self.__add_instr('map', 0, 0)
        self.__add_instr('sethndl', 0, 0)
        self.__add_instr('stobj2', self.__get_encoding_id(dict_name), 0)

        for key, value in itertools.izip(node.keys, node.values):
            key_name = self.__get_random_name()
            self.visit(key)
            self.__add_instr('stobj2', self.__get_encoding_id(key_name), 0)

            value_name = self.__get_random_name()
            self.visit(value)
            self.__add_instr('stobj2', self.__get_encoding_id(value_name), 0)

            self.__add_instr('ldobj2', self.__get_encoding_id(dict_name), 0)
            self.__add_instr('gethndl', 0, 0)
            self.__add_instr('ldobj2', self.__get_encoding_id(key_name), 0)
            self.__add_instr('gethndl', 0, 0)
            self.__add_instr('ldobj2', self.__get_encoding_id(value_name), 0)
            self.__add_instr('putobj', 0, 0)

            self.__add_instr('mapput', 0, 0)

            self.__add_instr('ldobj2', self.__get_encoding_id(dict_name), 0)
            self.__add_instr('sethndl', 0, 0)

    def visit_Compare(self, node):
        # Note: Only supports one comparison now.
        self.visit(node.comparators[0])
        self.visit(node.left)
        self.visit(node.ops[0])

    def visit_Call(self, node):
        self.visit(node.func)
        self.__add_instr('pinvk', 0, 0)

        # explicit args
        for arg in node.args:
            self.visit(arg)
            self.__add_instr('putarg', 0, 0)

        # implicit args
        if node.starargs:
            self.visit(node.starargs)
            self.__add_instr('putargs', 0, 0)

        self.__add_instr('invk', 0, 0)

    def visit_List(self, node):
        random_name = self.__get_random_name()

        self.__add_instr('new', 0, 0)
        self.__add_instr('ary', 0, 0)
        self.__add_instr('sethndl', 0, 0)
        self.__add_instr('stobj2', self.__get_encoding_id(random_name), 0)

        for item in node.elts:
            tmp_name = self.__get_random_name()
            self.visit(item)
            self.__add_instr('stobj2', self.__get_encoding_id(tmp_name), 0)
            self.__add_instr('ldobj2', self.__get_encoding_id(random_name), 0)
            self.__add_instr('gethndl', 0, 0)
            self.__add_instr('ldobj2', self.__get_encoding_id(tmp_name), 0)
            self.__add_instr('putobj', 0, 0)
            self.__add_instr('aryapnd', 0, 0)
            self.__add_instr('ldobj2', self.__get_encoding_id(random_name), 0)
            self.__add_instr('sethndl', 0, 0)

        self.__add_instr('ldobj2', self.__get_encoding_id(random_name), 0)

    def visit_Tuple(self, node):
        random_name = self.__get_random_name()

        self.__add_instr('new', 0, 0)
        self.__add_instr('ary', 0, 0)
        self.__add_instr('sethndl', 0, 0)
        self.__add_instr('stobj2', self.__get_encoding_id(random_name), 0)

        for item in node.elts:
            tmp_name = self.__get_random_name()
            self.visit(item)
            self.__add_instr('stobj2', self.__get_encoding_id(tmp_name), 0)
            self.__add_instr('ldobj2', self.__get_encoding_id(random_name), 0)
            self.__add_instr('gethndl', 0, 0)
            self.__add_instr('ldobj2', self.__get_encoding_id(tmp_name), 0)
            self.__add_instr('putobj', 0, 0)
            self.__add_instr('aryapnd', 0, 0)
            self.__add_instr('ldobj2', self.__get_encoding_id(random_name), 0)
            self.__add_instr('sethndl', 0, 0)

        self.__add_instr('ldobj2', self.__get_encoding_id(random_name), 0)

    def visit_Num(self, node):
        num_type = 'dec2' if isinstance(node.n, float) else 'int64'

        self.__add_instr('new', 0, 0, loc=Loc.from_node(node))

        if isinstance(node.n, int):
            self.__add_instr(num_type, node.n, 0, loc=Loc.from_node(node))
        else:
            # If the number if a float, split the number into its integer
            # and decimal parts, and express the decimal part in reverse order.
            integer_part = int(node.n)
            decimal_part = int(str(node.n).split('.')[1][::-1])
            self.__add_instr(num_type, integer_part, decimal_part, loc=Loc.from_node(node))

        self.__add_instr('sethndl', 0, 0, loc=Loc.from_node(node))

    def visit_Name(self, node):
        name = node.id

        if isinstance(node.ctx, ast.Load):
            self.__add_instr('ldobj', self.__get_encoding_id(name), 0, loc=Loc.from_node(node))
        elif isinstance(node.ctx, ast.Name):
            self.__add_instr('ldobj', self.__get_encoding_id(name), 0, loc=Loc.from_node(node))
        elif isinstance(node.ctx, ast.Param):
            # For loading parameters
            # Note: here we only want to handle args. kwargs are handled
            # differently in `visit_arguments`.
            self.__add_instr('getarg', 0, 0, loc=Loc.from_node(node))
            self.__add_instr('stobj', self.__get_encoding_id(name), 0, loc=Loc.from_node(node))
        elif isinstance(node.ctx, ast.Store):
            self.__add_instr('stobj', self.__get_encoding_id(name), 0, loc=Loc.from_node(node))
        else:
            # TODO: Add support for other types of ctx of `Name` node.
            pass

    def visit_Str(self, node):
        if not VectorString.is_vector_string(node.s):
            self.__add_instr('new', 0, 0, loc=Loc.from_node(node))
            self.__add_instr('str', self.__get_encoding_id(node.s), 0, loc=Loc.from_node(node))
            self.__add_instr('sethndl', 0, 0, loc=Loc.from_node(node))
        else:
            raw_vector = VectorString(node.s).to_raw_vector()

            for raw_instr in raw_vector:
                if len(raw_instr) < 3:
                    continue

                code, oprd1, oprd2 = self.__process_raw_instr(raw_instr)

                self.__add_instr(code, oprd1, oprd2, loc=Loc.from_node(node))

    def visit_Attribute(self, node):
        self.visit(node.value)
        if isinstance(node.ctx, ast.Load):
            self.__add_instr('getattr', self.__get_encoding_id(node.attr), 0, loc=Loc.from_node(node))
        elif isinstance(node.ctx, ast.Store):
            self.__add_instr('swap', 0, 0)
            self.__add_instr('setattr', self.__get_encoding_id(node.attr), 0, loc=Loc.from_node(node))
        else:
            # NOTE: if this ever happens, then we have to take that type into
            # consideration.
            raise Exception('Unexpected type for node.ctx in visit_Attribute')

    """ ---------------------------- boolop -------------------------------- """

    def visit_And(self, node):
        pass

    def visit_Or(self, node):
        pass

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
        pass

    def visit_Sub(self, node):
        pass

    def visit_Mult(self, node):
        pass

    def visit_Div(self, node):
        pass

    def visit_Mod(self, node):
        pass

    def visit_Pow(self, node):
        pass

    def visit_LShift(self, node):
        pass

    def visit_RShift(self, node):
        pass

    def visit_BitOr(self, node):
        pass

    def visit_BitXor(self, node):
        pass

    def visit_BitAnd(self, node):
        pass

    def visit_FloorDiv(self, node):
        pass

    """ ---------------------------- unaryop ------------------------------- """

    def visit_Invert(self, node):
        pass

    def visit_Not(self, node):
        pass

    def visit_UAdd(self, node):
        pass

    def visit_USub(self, node):
        pass

    """ ----------------------------- cmpop -------------------------------- """

    def visit_Eq(self, node):
        pass

    def visit_NotEq(self, node):
        pass

    def visit_Lt(self, node):
        pass

    def visit_LtE(self, node):
        pass

    def visit_Gt(self, node):
        pass

    def visit_GtE(self, node):
        pass

    def visit_Is(self, node):
        # TODO: logic can be placed under `object.__eq__` once
        # dynamic dispatching is supported.
        self.__add_instr('objeq', 0, 0)
        self.__add_instr('cldobj',
            self.__get_encoding_id('True'), self.__get_encoding_id('False'))

    def visit_IsNot(self, node):
        # TODO: logic can be placed under `object.__eq__` once
        # dynamic dispatching is supported.
        self.__add_instr('objneq', 0, 0)
        self.__add_instr('cldobj',
            self.__get_encoding_id('True'), self.__get_encoding_id('False'))

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
                self.__add_instr('getkwarg', self.__get_encoding_id(arg.id), 2, loc=Loc.from_node(node))
                self.__add_instr('jmp', 0, 0) # offset addr to be set
                vector_length1 = len(self.__current_vector())
                self.visit(default)
                vector_length2 = len(self.__current_vector())
                length_diff = vector_length2 - vector_length1
                self.__current_vector()[vector_length1 - 1] = Instr(
                    self.instr_str_to_code_map['jmp'], length_diff + 1, 0, loc=Loc.from_node(node))
            else:
                # This is an arg. It's handled in `visit_Name()`.
                self.visit(arg)

        # Pull out rest of the args (*args).
        if node.vararg:
            self.__add_instr('getargs', 0, 0)
            self.__add_instr('new', 0, 0)
            self.__add_instr('sethndl', 0, 0)
            self.__add_instr('stobj', self.__get_encoding_id(node.vararg), 0)

        # Pull out rest of the kwargs (**kwarg).
        if node.kwarg:
            self.__add_instr('getkwargs', 0, 0, loc=Loc.from_node(node))
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

        # Loads the built-in definitions.
        # NOTE: The definition of `bool` has to come before all other types.
        # because it defines the names `True` and `False` that are needed in
        # the logic.
        generator.read_from_source('python/src/__builtin__.py')
        generator.read_from_source('python/src/bool.py')
        generator.read_from_source('python/src/none.py')
        generator.read_from_source('python/src/int.py')
        generator.read_from_source('python/src/float.py')
        generator.read_from_source('python/src/str.py')
        generator.read_from_source('python/src/list.py')
        generator.read_from_source('python/src/dict.py')
        generator.read_from_source('python/src/tuple.py')

        generator.read_from_source(options.input_file)

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
