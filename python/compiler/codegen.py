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

import ast
import itertools
import optparse
import os
import pprint
import random
import json
import string
import sys
import time
import traceback

from datetime import datetime

import avro.datafile
import avro.io
import avro.schema


## -----------------------------------------------------------------------------

SCHEMAS_DIR = 'schemas/'
COREVM_BYTECODE_SCHEMA_FILE = 'corevm_bytecode_schema.json'
INSTR_STR_TO_CODE_MAP = 'INSTR_STR_TO_CODE_MAP'
DYOBJ_FLAG_STR_TO_VALUE_MAP = 'DYOBJ_FLAG_STR_TO_VALUE_MAP'
COREVM_BYTECODE_SCHEMA = os.path.join(SCHEMAS_DIR, COREVM_BYTECODE_SCHEMA_FILE)

## -----------------------------------------------------------------------------

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

## -----------------------------------------------------------------------------

class Instr(object):

    def __init__(self, code, oprd1, oprd2):
        self.code = code
        self.oprd1 = oprd1
        self.oprd2 = oprd2

    def to_json(self):
        return {
            'code': self.code,
            'oprd1': self.oprd1,
            'oprd2': self.oprd2
        }

## -----------------------------------------------------------------------------

class Loc(object):

    def __init__(self, index, lineno, col_offset):
        self.index = index
        self.lineno = lineno
        self.col_offset = col_offset

    def to_json(self):
        return {
            'index': self.index,
            'lineno': self.lineno,
            'col_offset': self.col_offset
        }

## -----------------------------------------------------------------------------

class CatchSite(object):

    def __init__(self, from_value, to_value, dst_value):
        self.from_value = int(from_value)
        self.to_value = int(to_value)
        self.dst_value = int(dst_value)

    def to_json(self):
        return {
            'from': self.from_value,
            'to': self.to_value,
            'dst': self.dst_value
        }

## -----------------------------------------------------------------------------

class TryExceptState(object):

    def __init__(self, closure_name, has_finally_block=False):
        self.in_except_block = False
        self.closure_name = closure_name
        self.has_finally_block = has_finally_block

## -----------------------------------------------------------------------------

class Scope(object):
    """Represents lexical scopes in source code.
    NOTE: top level modules do not have scopes, only classes and functions
    have scopes."""

    def __init__(self, in_cls=False, name=None):
        self.in_cls = in_cls
        self.name = name

## -----------------------------------------------------------------------------

class Closure(object):

    # Closure IDs are zero-based indices.
    __closure_id = 0

    # Default id for root closure.
    NONESET_CLOSURE_ID = -1

    required_fields = (
        'name',
        'id',
        'parent_id',
        'vector',
        'locs'
    )

    def __init__(self, original_name, name, parent_name, parent_id):
        self.original_name = original_name
        self.name = name
        self.parent_name = parent_name
        self.vector = []
        self.locs = []
        self.catch_sites = []
        self.closure_id = Closure.__closure_id
        self.parent_id = parent_id
        self.globals = []
        Closure.__closure_id += 1

    def add_loc(self, loc):
        self.locs.append(loc)

    def to_json(self):
        return {
            'name': self.original_name,
            'id': self.closure_id,
            'parent_id': self.parent_id if self.parent_id is not None else self.NONESET_CLOSURE_ID,
            'vector': [instr.to_json() for instr in self.vector],
            'locs': [loc.to_json() for loc in self.locs],
            'catch_sites': [catch_site.to_json() for catch_site in self.catch_sites]
        }

## -----------------------------------------------------------------------------

class EncodingMap(object):

    def __init__(self):
        self.encoding_id = 0
        self.inner = dict()

    def get_encoding_id(self, value):
        if value not in self.inner:
            self.inner[value] = self.encoding_id
            self.encoding_id += 1

        return self.inner[value]

    def flatten_to_table(self):
        flipped_encoding_map = dict((value, key)
            for key, value in self.inner.iteritems())

        if flipped_encoding_map:
            assert flipped_encoding_map.keys()[0] == 0

        return [
            flipped_encoding_map[key] for key in sorted(flipped_encoding_map)
        ]

## -----------------------------------------------------------------------------

class BytecodeValidationException(Exception):
    pass

## -----------------------------------------------------------------------------

class StructuredBytecodeValidator(object):

    def __init__(self, field, required=False):
        self.field = field
        self.required = required

    def validate(self, structured_bytecode):
        field_value = structured_bytecode.get(self.field)

        if field_value is None and self.required:
            raise BytecodeValidationException(
                'Missing expected field "%s" in bytecode' % self.field)

        return field_value

## -----------------------------------------------------------------------------

class ClosureTableValidator(StructuredBytecodeValidator):

    def validate(self, structured_bytecode):
        field_value = super(ClosureTableValidator, self).validate(structured_bytecode)

        for closure in field_value:
            self.validate_required_fields(closure)

        first_closure_id = field_value[0]['id']
        if first_closure_id != 0:
            raise BytecodeValidationException('Invalid value of field "id" in closure')

        closures_count = len(field_value)

        last_closure_id = field_value[-1]['id']
        if last_closure_id != closures_count - 1:
            raise BytecodeValidationException('Invalid value of field "id" in closure')

        # Check closures are stored in increasing order by their IDs.
        for i in xrange(1, closures_count):
            previous_closure = field_value[i - 1]
            next_closure = field_value[i]
            if previous_closure['id'] + 1 != next_closure['id']:
                raise BytecodeValidationException('Closures are not in order')

        # Check parent IDs.
        for closure in field_value:
            self.validate_closure_parent(closure, closures_count)

        # Check LOCs.
        for closure in field_value:
            self.validate_locs(closure)

        # Check catch sites.
        for closure in field_value:
            self.validate_catch_sites(closure)

    def validate_required_fields(self, closure):
        for required_field in Closure.required_fields:
            if required_field not in closure:
                raise BytecodeValidationException(
                    'Missing required field "%s" in closure' % required_field)

    def validate_closure_parent(self, closure, closures_count):
        parent_id = closure.get('parent_id')

        if parent_id == Closure.NONESET_CLOSURE_ID:
            return

        if parent_id < 0 or parent_id >= closures_count:
            raise BytecodeValidationException(
                'Invalid value of field "parent_id" in closure')

    def validate_locs(self, closure):
        locs = closure.get('locs')

        if locs:
            vector = closure['vector']
            vector_len = len(vector)

            for loc in locs:
                loc_index = loc.get('index')
                if loc_index is None:
                    raise BytecodeValidationException(
                        'Missing required field "index" in loc')
                else:
                    if loc_index < 0 or loc_index >= vector_len:
                        raise BytecodeValidationException(
                            'Invalid value of field "index" in loc')

                if loc.get('lineno') is None:
                    raise BytecodeValidationException(
                        'Missing required field "lineno" in loc')

                if loc.get('col_offset') is None:
                    raise BytecodeValidationException(
                        'Missing required field "col_offset" in loc')

            index_to_loc = dict((loc['index'], loc) for loc in locs)

            if len(index_to_loc) != len(locs):
                raise BytecodeValidationException('Duplicate LOC info in closure')

    def validate_catch_sites(self, closure):
        catch_sites = closure.get('catch_sites')

        if catch_sites:
            for catch_site in catch_sites:
                catch_site_from = catch_site.get('from')
                if catch_site_from is None:
                    raise BytecodeValidationException(
                        'Missing required field "from" in catch site')

                catch_site_to = catch_site.get('to')
                if catch_site_to is None:
                    raise BytecodeValidationException(
                        'Missing required field "to" in catch site')

                catch_site_dst = catch_site.get('dst')
                if catch_site_dst is None:
                    raise BytecodeValidationException(
                        'Missing required field "dst" in catch site')

                if not (catch_site_from < catch_site_to < catch_site_dst):
                    raise BytecodeValidationException(
                        'Invalid catch site fields')

## -----------------------------------------------------------------------------

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

    structured_bytecode_validators = (
        ClosureTableValidator('__MAIN__', required=True),
    )

    class NameManglingMode:
        NONE = 0x00
        MANGLE_BUILTIN = 0x01

    def __init__(self, options):
        self.input_file = options.input_file
        self.output_file = options.output_file
        self.debug_mode = options.debug_mode
        self.name_mangling_mode = self.NameManglingMode.NONE

        # Read info file
        with open(options.metadata_file, 'r') as fd:
            info_json = json.load(fd)

        self.instr_str_to_code_map = info_json[INSTR_STR_TO_CODE_MAP]
        self.dyobj_flag_str_to_value_map = info_json[DYOBJ_FLAG_STR_TO_VALUE_MAP]

        # encoding maps
        self.string_literal_encoding_map = EncodingMap()
        self.fpt_literal_encoding_map = EncodingMap()

        # closure map
        self.current_closure_name = self.default_closure_name
        self.closure_map = {
            self.current_closure_name: Closure(
                self.current_closure_name, self.current_closure_name, '', None)
        }

        # states
        self.current_class_name = ''
        self.current_function_name = ''
        self.try_except_states = []
        self.current_try_except_lvl = 0
        self.scopes = []
        self.continue_stmt_vector_lengths = []
        self.break_stmt_vector_lengths = []

    def read_from_source(self, path):
        with open(path, 'r') as fd:
            tree = ast.parse(fd.read())

        self.visit(tree)

    def set_name_mangling_mode(self, mode):
        self.name_mangling_mode = mode

    def finalize(self):
        with open(COREVM_BYTECODE_SCHEMA, 'r') as schema_file:
            bytecode_schema = avro.schema.parse(schema_file.read())

        structured_bytecode = {
            'format': self.format,
            'format_version': self.format_version,
            'target_version': self.target_version,
            'path': self.input_file,
            'timestamp': int(time.mktime(datetime.now().timetuple())),
            'encoding': self.encoding,
            'author': self.author,
            'string_literal_table': self.string_literal_encoding_map.flatten_to_table(),
            'fpt_literal_table': self.fpt_literal_encoding_map.flatten_to_table(),
            '__MAIN__': self.__finalize_closure_table()
        }

        self.__validate(structured_bytecode)

        with open(self.output_file, 'w') as fd:
            writer = avro.datafile.DataFileWriter(
                fd, avro.io.DatumWriter(), bytecode_schema)
            writer.append(structured_bytecode)
            writer.close()

    def __validate(self, structured_bytecode):
        """Performs validations on the structured bytecode before it gets
        serialized to output.
        """
        for validator in self.structured_bytecode_validators:
            validator.validate(structured_bytecode)

    def __finalize_closure_table(self):
        closure_table = sorted(
            self.closure_map.values(),
            key = lambda closure: closure.closure_id)

        if closure_table:
            assert (closure_table[0].closure_id == 0 and
                closure_table[-1].closure_id == len(closure_table) - 1)

        # Closures do not contain their parent names when emitted to bytecode.
        # This is a sanity check to make sure that their relationships match
        # by names.
        for closure in closure_table:
            if closure.parent_id is not None:
                parent_closure = closure_table[closure.parent_id]
                assert closure.parent_name == parent_closure.name, \
                    'Invalid hierarchical relation between closures'

        return [closure.to_json() for closure in closure_table]

    def __mangle_class_name(self, name):
        if self.name_mangling_mode == self.NameManglingMode.MANGLE_BUILTIN:
            return '_builtin_cls_' + name
        return name

    def __mangle_function_name(self, name):
        if self.name_mangling_mode == self.NameManglingMode.MANGLE_BUILTIN:
            return '_builtin_func_' + name
        return name

    def __enter_scope(self, in_cls=False, name=None):
        self.scopes.append(Scope(in_cls=in_cls, name=name))

    def __exit_scope(self):
        self.scopes.pop()

    def __current_scope(self):
        return self.scopes[-1] if self.scopes else None

    def __module_frame_lvl(self):
        """Returns the `n`th top frame lvl of the module lvl frame.
        Note that only scopes associated with functions introduce frames at
        runtime. Each scope introduces 2 frames because of Python wrapper calls.
        """
        return sum([0 if scope.in_cls else 2 for scope in self.scopes])

    def __current_closure(self):
        return self.closure_map[self.current_closure_name]

    def __current_vector(self):
        return self.__current_closure().vector

    def __add_try_except_state(self, try_except_state):
        self.try_except_states.append(try_except_state)

    def __pop_try_except_state(self):
        assert self.current_try_except_lvl > 0, \
            'Current try-except level should not be zero'

        assert len(self.try_except_states) > 0, \
            'Try-except states should not be empty'

        assert self.current_try_except_lvl <= len(self.try_except_states), \
            'Invalid try-except states'

        self.current_try_except_lvl -= 1
        self.try_except_states.pop()

    def __current_try_except_state(self):
        assert len(self.try_except_states), 'No try-except states found'
        return self.try_except_states[-1]

    def __total_try_except_lvl(self):
        return len(self.try_except_states)

    def __add_instr(self, code, oprd1, oprd2, node=None):
        self.__current_vector().append(
            Instr(
                self.instr_str_to_code_map[code],
                oprd1,
                oprd2
            )
        )

        if node:
            self.__add_loc_from_node(node)

    def __add_loc_from_node(self, node):
        self.__current_closure().add_loc(
            Loc(len(self.__current_vector()) - 1, node.lineno, node.col_offset))

    def __add_catch_site(self, catch_site):
        self.__current_closure().catch_sites.append(catch_site)

    def __get_string_literal_encoding_id(self, name):
        return self.string_literal_encoding_map.get_encoding_id(name)

    def __get_fpt_literal_encoding_id(self, fpt):
        return self.fpt_literal_encoding_map.get_encoding_id(fpt)

    def __get_dyobj_flag(self, flags):
        value = 0

        for flag in flags:
            value |= (1 << self.dyobj_flag_str_to_value_map[flag])

        return value

    def __process_raw_instr(self, raw_instr):
        INSTRS_NEED_ENCODING_ID = (
            'ldobj',
            'stobj',
            'cldobj',
            'getval2',
            'setattr',
            'getattr',
            'getarg',
            'putkwarg',
            'getkwarg',
            'rsetattrs',
            'setattrs',
            'str',
            'dbgvar'
        )

        INSTRS_NEED_ENCODING_ID_2 = (
            'cldobj'
        )

        raw_code, raw_oprd1, raw_oprd2 = raw_instr

        raw_code = raw_code.strip()
        raw_oprd1 = raw_oprd1.strip()
        raw_oprd2 = raw_oprd2.strip()

        code = raw_code.strip()

        if raw_code in INSTRS_NEED_ENCODING_ID:
            oprd1 = self.__get_string_literal_encoding_id(raw_oprd1)
        else:
            oprd1 = int(raw_oprd1)

        if raw_code in INSTRS_NEED_ENCODING_ID_2:
            oprd2 = self.__get_string_literal_encoding_id(raw_oprd2)
        else:
            oprd2 = int(raw_oprd2)

        return code, oprd1, oprd2

    def __get_random_name(self):
        return ''.join(random.choice(string.ascii_letters) for _ in xrange(5))

    def __enter_class_name_mangling(self, name):
        self.current_class_name = \
            self.current_class_name + '::' + self.__mangle_class_name(name)

    def __exit_class_name_mangling(self):
        self.current_class_name = '::'.join(self.current_class_name.split('::')[:-1])

    def __enter_function_name_mangling(self, name):
        self.current_function_name = \
            self.current_function_name + '::' + self.__mangle_function_name(name)

    def __exit_function_name_mangling(self):
        self.current_function_name = '::'.join(self.current_function_name.split('::')[:-1])

    def __enter_closure_in_function(self):
        if self.current_class_name:
            closure_name = self.current_class_name + '.' + self.current_function_name
        else:
            closure_name = self.current_function_name

        return closure_name

    """ ----------------------------- stmt --------------------------------- """

    def visit_FunctionDef(self, node):
        # Step in.
        self.__enter_scope()
        self.__enter_function_name_mangling(node.name)
        closure_name = self.__enter_closure_in_function()

        # Check if a function with the same name already exists under the same
        # scope. We don't want to create another closure if it already exists.
        if closure_name not in self.closure_map:
            self.closure_map[closure_name] = Closure(
                node.name,
                closure_name,
                self.current_closure_name,
                self.closure_map[self.current_closure_name].closure_id
            )
        else:
            # If a function with the same name already exists under the same scope,
            # use the existing closure, but clears some of its members before
            # adding instructions.
            self.closure_map[closure_name].vector = []
            self.closure_map[closure_name].locs = []
            self.closure_map[closure_name].catch_sites = []

        self.current_closure_name = closure_name

        # Off-load arguments.
        self.visit(node.args)

        # Statements.
        for stmt in node.body:
            self.visit(stmt)

        # Explicit return.
        self.__add_instr('rtrn', 0, 0)

        # Step out.
        self.__exit_scope()
        self.current_closure_name = self.closure_map[self.current_closure_name].parent_name
        self.__exit_function_name_mangling()

        # In the outer closure, set the closure id on the object

        self.__add_instr('new', self.__get_dyobj_flag(['DYOBJ_IS_NOT_GARBAGE_COLLECTIBLE']), 0)
        self.__add_instr('setctx', self.closure_map[closure_name].closure_id, 0)
        self.__add_instr('ldobj', self.__get_string_literal_encoding_id('function'), 0)
        self.__add_instr('setattr', self.__get_string_literal_encoding_id('__class__'), 0)

        if not self.current_class_name:
            if node.name in self.__current_closure().globals:
                n = self.__module_frame_lvl()
                self.__add_instr('stobjn', self.__get_string_literal_encoding_id(node.name), n)
            else:
                self.__add_instr('stobj', self.__get_string_literal_encoding_id(node.name), 0)

    def visit_ClassDef(self, node):
        # Step in.
        self.__enter_class_name_mangling(node.name)
        self.__enter_scope(in_cls=True, name=self.current_class_name)

        self.__add_instr('new', self.__get_dyobj_flag(['DYOBJ_IS_NOT_GARBAGE_COLLECTIBLE']), 0)
        if node.name in self.__current_closure().globals:
            n = self.__module_frame_lvl()
            self.__add_instr('stobjn', self.__get_string_literal_encoding_id(node.name), n)
        else:
            self.__add_instr('stobj', self.__get_string_literal_encoding_id(node.name), 0)
        self.__add_instr('ldobj', self.__get_string_literal_encoding_id(node.name), 0)
        self.__add_instr('ldobj', self.__get_string_literal_encoding_id('type'), 0)
        self.__add_instr('setattr', self.__get_string_literal_encoding_id('__class__'), 0)

        # Also store class object to an invisible variable.
        self.__add_instr('ldobj', self.__get_string_literal_encoding_id(node.name), 0)
        self.__add_instr('stobj2', self.__get_string_literal_encoding_id(self.current_class_name), 0)

        for stmt in node.body:
            if isinstance(stmt, ast.FunctionDef):
                tmp_name = self.__get_random_name()
                self.visit(stmt)
                self.__add_instr('ldobj', self.__get_string_literal_encoding_id('MethodType'), 0)
                self.__add_instr('setattr', self.__get_string_literal_encoding_id('__class__'), 0)
                self.__add_instr('stobj2', self.__get_string_literal_encoding_id(tmp_name), 0)
                self.__add_instr('ldobj2', self.__get_string_literal_encoding_id(self.current_class_name), 0)
                self.__add_instr('ldobj2', self.__get_string_literal_encoding_id(tmp_name), 0)
                self.__add_instr('setattr', self.__get_string_literal_encoding_id(stmt.name), 0)
            elif isinstance(stmt, ast.ClassDef):
                tmp_name = self.__get_random_name()
                self.visit(stmt)
                self.__add_instr('stobj2', self.__get_string_literal_encoding_id(tmp_name), 0)
                self.__add_instr('ldobj2', self.__get_string_literal_encoding_id(self.current_class_name), 0)
                self.__add_instr('ldobj2', self.__get_string_literal_encoding_id(tmp_name), 0)
                self.__add_instr('setattr', self.__get_string_literal_encoding_id(stmt.name), 0)
            else:
                self.visit(stmt)

        # Step out.
        # Restore class object in scope with its original name.
        self.__add_instr('ldobj2', self.__get_string_literal_encoding_id(self.current_class_name), 0)
        self.__add_instr('stobj', self.__get_string_literal_encoding_id(node.name), 0)
        self.__exit_scope()
        self.__exit_class_name_mangling()

    def visit_Return(self, node):
        # TODO: [COREVM-176] Support return value in Python
        if node.value:
            self.visit(node.value)
        self.__add_instr('rtrn', 0, 0)

    def visit_Delete(self, node):
        for target in node.targets:
            # `del` statements on non-name targets have been transformed into
            # function calls.
            assert isinstance(target, ast.Name)

            self.__add_instr('delobj', self.__get_string_literal_encoding_id(target.id), 0)

    def visit_Assign(self, node):
        res = self.visit(node.value)

        if isinstance(node.value, ast.Lambda):
            assert res # This is the name of the lambda closure.
            self.__add_instr('setctx', self.closure_map[res].closure_id, 0)

        self.visit(node.targets[0])

    def visit_AugAssign(self, node):
        pass

    def visit_Print(self, node):
        assert len(node.values) == 1
        self.visit(node.values[0])
        self.__add_instr('print', 0, 0, node=node)

    def visit_For(self, node):
        # TODO: Handle `orelse` here.
        index_name = self.__get_random_name()
        iter_name = self.__get_random_name()

        # Store index object into a variable.
        self.__add_instr('new', 0, 0)
        self.__add_instr('uint32', 0, 0)
        self.__add_instr('setval', 0, 0)
        self.__add_instr('stobj2', self.__get_string_literal_encoding_id(index_name), 0)

        # Store iter object into a variable.
        self.visit(node.iter)
        self.__add_instr('stobj2', self.__get_string_literal_encoding_id(iter_name), 0)

        vector_length1 = len(self.__current_vector())

        self.__add_instr('ldobj2', self.__get_string_literal_encoding_id(iter_name), 0)
        self.__add_instr('getval', 0, 0)
        self.__add_instr('arylen', 0, 0)
        self.__add_instr('ldobj2', self.__get_string_literal_encoding_id(index_name), 0)
        self.__add_instr('getval', 0, 0)
        self.__add_instr('gte', 0, 0) # The opposite of `lt` check.

        self.__add_instr('jmpif', 0, 0)
        vector_length2 = len(self.__current_vector())

        # Get item from array, and load into target.
        self.__add_instr('ldobj2', self.__get_string_literal_encoding_id(iter_name), 0)
        self.__add_instr('getval', 0, 0)
        self.__add_instr('ldobj2', self.__get_string_literal_encoding_id(index_name), 0)
        self.__add_instr('getval', 0, 0)
        self.__add_instr('aryat', 0, 0)
        self.__add_instr('getobj', 0, 0)
        self.__add_instr('stobj', self.__get_string_literal_encoding_id(node.target.id), 0)

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
        self.__add_instr('ldobj2', self.__get_string_literal_encoding_id(index_name), 0)
        self.__add_instr('getval', 0, 0)
        self.__add_instr('inc', 0, 0)
        self.__add_instr('setval', 0, 0)
        self.__add_instr('stobj2', self.__get_string_literal_encoding_id(index_name), 0)
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
        vector_length1 = len(self.__current_vector())
        self.continue_stmt_vector_lengths.append(vector_length1)

        break_line_length_count_before = len(self.break_stmt_vector_lengths)

        # Test expr.
        self.visit(node.test)

        self.__add_instr('istruthy', 0, 0)
        self.__add_instr('lnot', 0, 0)
        self.__add_instr('jmpif', 0, 0)

        vector_length2 = len(self.__current_vector())

        break_line_length = 0

        # Execute body instruction.
        for stmt in node.body:
            self.visit(stmt)

        # Jump back to beginning.
        self.__add_instr('jmpr', vector_length1 - 1, 0)

        vector_length3 = len(self.__current_vector())

        # Execute `else` part.
        if node.orelse:
            for stmt in node.orelse:
                self.visit(stmt)

        vector_length4 = len(self.__current_vector())

        length_diff = vector_length3 - vector_length2
        self.__current_vector()[vector_length2 - 1] = Instr(
            self.instr_str_to_code_map['jmpif'], length_diff, 0)

        break_line_length_count_after = len(self.break_stmt_vector_lengths)

        # If there is a break stmt associated with this while-loop.
        if break_line_length_count_after > break_line_length_count_before:
            break_line_length = self.break_stmt_vector_lengths[-1]
            break_length_diff = vector_length4 - break_line_length

            self.__current_vector()[break_line_length - 1] = Instr(
                self.instr_str_to_code_map['jmp'], break_length_diff, 0)

            self.break_stmt_vector_lengths.pop()

        self.continue_stmt_vector_lengths.pop()

    def visit_If(self, node):
        """
        if condition:
            (1)
            .
            .
            .
            (3)
        else:
            (2)
            .
            .
            .
            (4)
        """
        self.visit(node.test)
        self.__add_instr('istruthy', 0, 0, node=node)
        self.__add_instr('lnot', 0, 0, node=node)

        # Add `jmpif` here.
        # `vector_length1` matches with `vector_length2` below.
        self.__add_instr('jmpif', 0, 0, node=node)
        vector_length1 = len(self.__current_vector())

        for stmt in node.body:
            self.visit(stmt)

        if node.orelse:
            # `vector_length3` matches with `vector_length4` below.
            self.__add_instr('jmp', 0, 0, node=node)
            vector_length3 = len(self.__current_vector())

        # Add `jmpif` here.
        vector_length2 = len(self.__current_vector())
        length_diff = vector_length2 - vector_length1
        self.__current_vector()[vector_length1 - 1].oprd1 = length_diff

        for stmt in node.orelse:
            self.visit(stmt)

        if node.orelse:
            vector_length4 = len(self.__current_vector())
            length_diff2 = vector_length4 - vector_length3
            self.__current_vector()[vector_length3 - 1].oprd1 = length_diff2

    def visit_With(self, node):
        pass

    def visit_Raise(self, node):
        self.visit(node.type)

        search_catch_sites = 1

        if not self.try_except_states:
            search_catch_sites = 0
        elif (
            self.try_except_states and
            self.__current_try_except_state().in_except_block
        ):
            # Do NOT turn on catch site searching in except blocks,
            # when there's no "finally" block under the same closure.
            has_finally_block_in_closure = False
            for try_except_state in self.try_except_states:
                if (
                    try_except_state.closure_name == self.current_closure_name and
                    try_except_state.has_finally_block
                ):
                    has_finally_block_in_closure = True
                    break

            search_catch_sites = int(has_finally_block_in_closure)

        self.__add_instr('exc', search_catch_sites, 0)

    def visit_TryExcept(self, node):
        if self.current_try_except_lvl == self.__total_try_except_lvl():
            self.__add_try_except_state(TryExceptState(self.current_closure_name))

        self.current_try_except_lvl += 1

        vector_length_try_block_before = len(self.__current_vector())

        for stmt in node.body:
            self.visit(stmt)

        vector_length_try_block_after = len(self.__current_vector())

        if node.handlers:
            self.__add_catch_site(
                CatchSite(
                    from_value=vector_length_try_block_before,
                    to_value=vector_length_try_block_after - 1,
                    dst_value=vector_length_try_block_after
                )
            )

        self.__add_instr('jmpexc', 0, 0)

        vector_length_handlers_before = len(self.__current_vector())

        # Do the work inside `visit_excepthandler` here instead.
        for i in xrange(len(node.handlers)):
            handler = node.handlers[i]

            vector_length_except_block_before = len(self.__current_vector())

            self.visit(handler.type)
            self.__add_instr('excobj', 0, 0)
            self.__add_instr('getattr', self.__get_string_literal_encoding_id('__class__'), 0)
            self.__add_instr('swap', 0, 0)
            self.__add_instr('objeq', 0, 0)

            self.__add_instr('jmpif', 0, 0)

            vector_length_x = len(self.__current_vector())

            self.__add_instr('excobj', 0, 0)
            self.__add_instr('exc', 1, 0)

            vector_length_y = len(self.__current_vector())

            length_diff = vector_length_y - vector_length_x
            self.__current_vector()[vector_length_x - 1] = Instr(
                self.instr_str_to_code_map['jmpif'], length_diff, 0)

            # Step in (for except-block stmts).
            self.__current_try_except_state().in_except_block = True

            if handler.name:
                self.__add_instr('excobj', 0, 0)
                self.visit(handler.name)

            for stmt in handler.body:
                self.visit(stmt)

            # Step out (for except-block stmts).
            self.__current_try_except_state().in_except_block = False

            vector_length_except_block_after = len(self.__current_vector())

            self.__add_catch_site(
                CatchSite(
                  from_value=vector_length_except_block_before,
                  to_value=vector_length_except_block_after - 1,
                  dst_value=vector_length_except_block_after
                )
            )

        vector_length_handlers_after = len(self.__current_vector())

        vector_length_handlers_diff = \
            vector_length_handlers_after - vector_length_handlers_before

        self.__current_vector()[vector_length_handlers_before - 1].oprd1 = \
            vector_length_handlers_diff

        self.__add_instr('jmpexc', 0, 1)

        vector_length_before_orelse = len(self.__current_vector())

        if node.orelse:
            for stmt in node.orelse:
                self.visit(stmt)

        vector_length_after_orelse = len(self.__current_vector())

        vector_length_orelse_diff = \
            vector_length_after_orelse - vector_length_before_orelse

        self.__current_vector()[vector_length_before_orelse - 1].oprd1 = \
            vector_length_orelse_diff

        self.__add_instr('clrexc', 0, 0)

        if not self.__current_try_except_state().has_finally_block:
            self.__pop_try_except_state()

    def visit_TryFinally(self, node):
        """
        Reference:
            https://docs.python.org/2/tutorial/errors.html#defining-clean-up-actions
        """
        self.__add_try_except_state(
            TryExceptState(self.current_closure_name, has_finally_block=True))

        for stmt in node.body:
            self.visit(stmt)

        vector_length = len(self.__current_vector())

        if self.__current_closure().catch_sites:
            self.__current_closure().catch_sites[-1].dst_value = vector_length

        for stmt in node.finalbody:
            self.visit(stmt)

        self.__add_instr('jmpexc', 2, 0)
        self.__add_instr('excobj', 0, 0)
        self.__add_instr('exc', 1, 0)

        self.__pop_try_except_state()

    def visit_Global(self, node):
        for name in node.names:
            self.__current_closure().globals.append(name)

    def visit_Expr(self, node):
        self.visit(node.value)

    def visit_Pass(self, node):
        pass

    def visit_Break(self, node):
        self.__add_instr('jmp', 0, 0)
        break_line_length = len(self.__current_vector())
        self.break_stmt_vector_lengths.append(break_line_length)

    def visit_Continue(self, node):
        assert self.continue_stmt_vector_lengths
        self.__add_instr('jmpr', self.continue_stmt_vector_lengths[-1] - 1, 0)

    """ ----------------------------- expr --------------------------------- """

    def visit_Ellipsis(self, node):
        raise NotImplementedError

    def visit_Slice(self, node):
        raise NotImplementedError

    def visit_ExtSlice(self, node):
        raise NotImplementedError

    def visit_Index(self, node):
        self.visit(node.value)

    def __process_logical_and_recursive(self, exprs):
        assert exprs
        expr = exprs[0]

        left_name = self.__get_random_name()
        right_name = self.__get_random_name()

        assert left_name != right_name

        left_name_id = self.__get_string_literal_encoding_id(left_name)
        right_name_id = self.__get_string_literal_encoding_id(right_name)

        self.visit(expr)

        jmp_lengths = []

        if len(exprs) - 1 > 0:
            self.__add_instr('stobj2', left_name_id, 0)

            self.visit(exprs[1])
            self.__add_instr('stobj2', right_name_id, 0)

            self.__add_instr('ldobj2', left_name_id, 0)
            self.__add_instr('getval', 0, 0)
            self.__add_instr('ldobj2', right_name_id, 0)
            self.__add_instr('getval', 0, 0)
            self.__add_instr('land', 0, 0)
            self.__add_instr('truthy', 0, 0)

            self.__add_instr('cldobj',
                self.__get_string_literal_encoding_id('True'), self.__get_string_literal_encoding_id('False'))

            self.__add_instr('lnot', 0, 0)
            self.__add_instr('jmpif', 0, 0)
            jmp_lengths.append(len(self.__current_vector()))

            if len(exprs[1:]) > 0:
                jmp_lengths.extend(self.__process_logical_and_recursive(exprs[1:]))

        return jmp_lengths

    def __process_logical_and(self, exprs):
        jmp_lengths = self.__process_logical_and_recursive(exprs)

        current_length = len(self.__current_vector())

        for jmp_length in jmp_lengths:
            length_diff = current_length - jmp_length

            i = jmp_length - 1
            assert self.__current_vector()[i].code == self.instr_str_to_code_map['jmpif']
            self.__current_vector()[i].oprd1 = length_diff

    def visit_BoolOp(self, node):
        assert len(node.values) >= 2

        left_name = self.__get_random_name()
        right_name = self.__get_random_name()

        assert left_name != right_name

        left_name_id = self.__get_string_literal_encoding_id(left_name)
        right_name_id = self.__get_string_literal_encoding_id(right_name)

        assert isinstance(node.op, ast.And) or isinstance(node.op, ast.Or)

        if isinstance(node.op, ast.Or):
            jmp_lengths = []

            self.visit(node.values[0])
            self.__add_instr('stobj2', left_name_id, 0)

            self.visit(node.values[1])
            self.__add_instr('stobj2', right_name_id, 0)

            self.__add_instr('ldobj2', left_name_id, 0)
            self.__add_instr('getval', 0, 0)
            self.__add_instr('ldobj2', right_name_id, 0)
            self.__add_instr('getval', 0, 0)
            self.__add_instr('lor', 0, 0)
            self.__add_instr('jmpif', 0, 0)

            jmp_lengths.append(len(self.__current_vector()))

            for i in xrange(2, len(node.values)):
                self.visit(node.values[i])

                self.__add_instr('getval', 0, 0)
                self.__add_instr('bool', 0, 0)
                self.__add_instr('lor', 0, 0)
                self.__add_instr('jmpif', 0, 0)

                jmp_lengths.append(len(self.__current_vector()))

            current_length = len(self.__current_vector())
            self.__add_instr('cldobj',
                self.__get_string_literal_encoding_id('True'), self.__get_string_literal_encoding_id('False'))

            for jmp_length in jmp_lengths:
                length_diff = current_length - jmp_length

                self.__current_vector()[jmp_length - 1] = Instr(
                    self.instr_str_to_code_map['jmpif'], length_diff, 0)
        elif isinstance(node.op, ast.And):
            self.__process_logical_and(node.values)

    def visit_BinOp(self, node):
        self.visit(node.right)
        self.visit(node.left)
        self.visit(node.op)

    def visit_Lambda(self, node):
        # This is very similar to `visit_FunctionDef`.

        # step in
        name = 'lambda_' + self.__get_random_name()

        assert name not in self.closure_map, 'Lambda closure names should be globally unique'

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

        self.__add_instr('new', self.__get_dyobj_flag(['DYOBJ_IS_NOT_GARBAGE_COLLECTIBLE']), 0)
        self.__add_instr('setctx', self.closure_map[name].closure_id, 0)
        self.__add_instr('ldobj', self.__get_string_literal_encoding_id('object'), 0)
        self.__add_instr('setattr', self.__get_string_literal_encoding_id('__class__'), 0)

        return name

    def visit_IfExp(self, node):
        self.visit(node.test)

        self.__add_instr('istruthy', 0, 0)
        self.__add_instr('jmpif', 0, 0)

        vector_length1 = len(self.__current_vector())

        self.visit(node.orelse)

        self.__add_instr('jmp', 0, 0)

        vector_length2 = len(self.__current_vector())

        self.visit(node.body)

        vector_length3 = len(self.__current_vector())

        assert self.__current_vector()[vector_length1 - 1].code == self.instr_str_to_code_map['jmpif']
        self.__current_vector()[vector_length1 - 1].oprd1 = vector_length2 - vector_length1

        assert self.__current_vector()[vector_length2 - 1].code == self.instr_str_to_code_map['jmp']
        self.__current_vector()[vector_length2 - 1].oprd1 = vector_length3 - vector_length2

    def visit_Dict(self, node):
        assert len(node.keys) == len(node.values)

        dict_name = self.__get_random_name()

        self.__add_instr('new', 0, 0)
        self.__add_instr('map', 0, 0)
        self.__add_instr('setval', 0, 0)
        self.__add_instr('stobj2', self.__get_string_literal_encoding_id(dict_name), 0)

        for key, value in itertools.izip(node.keys, node.values):
            key_name = self.__get_random_name()
            self.visit(key)
            self.__add_instr('stobj2', self.__get_string_literal_encoding_id(key_name), 0)

            value_name = self.__get_random_name()
            self.visit(value)
            self.__add_instr('stobj2', self.__get_string_literal_encoding_id(value_name), 0)

            # Value.
            self.__add_instr('ldobj2', self.__get_string_literal_encoding_id(value_name), 0)
            self.__add_instr('putobj', 0, 0)

            # Key.
            self.__add_instr('ldobj2', self.__get_string_literal_encoding_id(key_name), 0)
            self.__add_instr('getval', 0, 0)

            # Dict.
            self.__add_instr('ldobj2', self.__get_string_literal_encoding_id(dict_name), 0)
            self.__add_instr('getval', 0, 0)

            self.__add_instr('mapput', 0, 0)

            self.__add_instr('setval', 0, 0)

        self.__add_instr('ldobj2', self.__get_string_literal_encoding_id(dict_name), 0)

    def visit_Set(self, node):
        set_name = self.__get_random_name()

        self.__add_instr('new', 0, 0)
        self.__add_instr('map', 0, 0)
        self.__add_instr('setval', 0, 0)
        self.__add_instr('stobj2', self.__get_string_literal_encoding_id(set_name), 0)

        for elt in node.elts:
            item_name = self.__get_random_name()

            self.visit(elt)
            self.__add_instr('stobj2', self.__get_string_literal_encoding_id(item_name), 0)

            # Value.
            self.__add_instr('ldobj2', self.__get_string_literal_encoding_id(item_name), 0)
            self.__add_instr('getattr', self.__get_string_literal_encoding_id('value'), 0)
            self.__add_instr('putobj', 0, 0)

            # Key.
            self.__add_instr('ldobj2', self.__get_string_literal_encoding_id(item_name), 0)
            self.__add_instr('getattr', self.__get_string_literal_encoding_id('hash'), 0)
            self.__add_instr('getval', 0, 0)

            # Set.
            self.__add_instr('ldobj2', self.__get_string_literal_encoding_id(set_name), 0)
            self.__add_instr('getval', 0, 0)

            self.__add_instr('mapput', 0, 0)

            self.__add_instr('setval', 0, 0)

        self.__add_instr('ldobj2', self.__get_string_literal_encoding_id(set_name), 0)

    def visit_ListComp(self, node):
        # Do nothing here.
        # Comprehension syntax gets unwrapped in code_transformer.py
        pass

    def visit_SetComp(self, node):
        # Do nothing here.
        # Comprehension syntax gets unwrapped in code_transformer.py
        pass

    def visit_DictComp(self, node):
        # Do nothing here.
        # Comprehension syntax gets unwrapped in code_transformer.py
        pass

    def visit_Compare(self, node):
        # Do nothing here.
        # Comparison expressions get unwrapped in code_transformer.py,
        # except for `is` and `is not`.
        assert len(node.ops) == len(node.comparators) == 1
        assert isinstance(node.ops[0], ast.Is) or isinstance(node.ops[0], ast.IsNot)
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

        # keyword args
        for keyword in node.keywords:
            self.visit(keyword.value)
            self.__add_instr('putkwarg', self.__get_string_literal_encoding_id(keyword.arg), 0)

        # implicit args
        if node.starargs:
            self.visit(node.starargs)
            self.__add_instr('putargs', 0, 0)

        # implicit kwargs
        if node.kwargs:
            self.visit(node.kwargs)
            self.__add_instr('putkwargs', 0, 0)

        self.__add_instr('invk', 0, 0)

    def visit_List(self, node):
        random_name = self.__get_random_name()

        self.__add_instr('new', 0, 0)
        self.__add_instr('ary', 0, 0)
        self.__add_instr('setval', 0, 0)
        self.__add_instr('stobj2', self.__get_string_literal_encoding_id(random_name), 0)

        for item in node.elts:
            tmp_name = self.__get_random_name()
            self.visit(item)
            self.__add_instr('stobj2', self.__get_string_literal_encoding_id(tmp_name), 0)
            self.__add_instr('ldobj2', self.__get_string_literal_encoding_id(random_name), 0)
            self.__add_instr('getval', 0, 0)
            self.__add_instr('ldobj2', self.__get_string_literal_encoding_id(tmp_name), 0)
            self.__add_instr('putobj', 0, 0)
            self.__add_instr('swap2', 0, 0)
            self.__add_instr('aryapnd', 0, 0)
            self.__add_instr('ldobj2', self.__get_string_literal_encoding_id(random_name), 0)
            self.__add_instr('setval', 0, 0)

        self.__add_instr('ldobj2', self.__get_string_literal_encoding_id(random_name), 0)

    def visit_Tuple(self, node):
        random_name = self.__get_random_name()

        self.__add_instr('new', 0, 0)
        self.__add_instr('ary', 0, 0)
        self.__add_instr('setval', 0, 0)
        self.__add_instr('stobj2', self.__get_string_literal_encoding_id(random_name), 0)

        for item in node.elts:
            tmp_name = self.__get_random_name()
            self.visit(item)
            self.__add_instr('stobj2', self.__get_string_literal_encoding_id(tmp_name), 0)
            self.__add_instr('ldobj2', self.__get_string_literal_encoding_id(random_name), 0)
            self.__add_instr('getval', 0, 0)
            self.__add_instr('ldobj2', self.__get_string_literal_encoding_id(tmp_name), 0)
            self.__add_instr('putobj', 0, 0)
            self.__add_instr('swap2', 0, 0)
            self.__add_instr('aryapnd', 0, 0)
            self.__add_instr('ldobj2', self.__get_string_literal_encoding_id(random_name), 0)
            self.__add_instr('setval', 0, 0)

        self.__add_instr('ldobj2', self.__get_string_literal_encoding_id(random_name), 0)

    def visit_Num(self, node):
        num_type = 'dec2' if isinstance(node.n, float) else 'int64'

        self.__add_instr('new', 0, 0, node=node)

        if isinstance(node.n, int):
            self.__add_instr(num_type, node.n, 0, node=node)
        else:
            encoding_id = self.__get_fpt_literal_encoding_id(node.n)
            self.__add_instr(num_type, encoding_id, 0, node=node)

        self.__add_instr('setval', 0, 0, node=node)

    def visit_Name(self, node):
        name = node.id

        if isinstance(node.ctx, ast.Load):
            self.__add_instr('ldobj', self.__get_string_literal_encoding_id(name), 0, node=node)
        elif isinstance(node.ctx, ast.Name):
            self.__add_instr('ldobj', self.__get_string_literal_encoding_id(name), 0, node=node)
        elif isinstance(node.ctx, ast.Param):
            # For loading parameters
            # Note: here we only want to handle args. kwargs are handled
            # differently in `visit_arguments`.
            self.__add_instr('getarg', self.__get_string_literal_encoding_id(name), 0, node=node)
        elif isinstance(node.ctx, ast.Store):
            current_scope = self.__current_scope()
            if current_scope and current_scope.in_cls:
                # In a class definition scope.
                self.__add_instr('stobj', self.__get_string_literal_encoding_id(name), 0)

                # Set the target name as the class's attribute as well.
                #
                # Steps:
                #   1. Load class.
                #   2. Load target name.
                #   3. Set attribute.
                assert current_scope.name, 'Expecting a class name'
                self.__add_instr('ldobj2', self.__get_string_literal_encoding_id(current_scope.name), 0, node=node)
                self.__add_instr('ldobj', self.__get_string_literal_encoding_id(name), 0, node=node)
                self.__add_instr('setattr', self.__get_string_literal_encoding_id(name), 0, node=node)
            elif name in self.__current_closure().globals:
                n = self.__module_frame_lvl()
                self.__add_instr('stobjn', self.__get_string_literal_encoding_id(name), n, node=node)
            else:
                self.__add_instr('stobj', self.__get_string_literal_encoding_id(name), 0, node=node)
        else:
            # TODO: Add support for other types of ctx of `Name` node.
            pass

    def visit_Str(self, node):
        if not VectorString.is_vector_string(node.s):
            self.__add_instr('new', 0, 0, node=node)
            self.__add_instr('str', self.__get_string_literal_encoding_id(node.s), 0, node=node)
            self.__add_instr('setval', 0, 0, node=node)
        else:
            raw_vector = VectorString(node.s).to_raw_vector()

            for raw_instr in raw_vector:
                if len(raw_instr) < 3:
                    continue

                code, oprd1, oprd2 = self.__process_raw_instr(raw_instr)

                self.__add_instr(code, oprd1, oprd2, node=node)

    def visit_Attribute(self, node):
        self.visit(node.value)
        if isinstance(node.ctx, ast.Load):
            self.__add_instr('getattr', self.__get_string_literal_encoding_id(node.attr), 0, node=node)
        elif isinstance(node.ctx, ast.Store):
            self.__add_instr('swap', 0, 0)
            self.__add_instr('setattr', self.__get_string_literal_encoding_id(node.attr), 0, node=node)
        else:
            # NOTE: if this ever happens, then we have to take that type into
            # consideration.
            raise Exception('Unexpected type for node.ctx in visit_Attribute')

    def visit_Subscript(self, node):
        pass

    """ ---------------------------- boolop -------------------------------- """

    def visit_And(self, node):
        pass

    def visit_Or(self, node):
        pass

    """ --------------------------- operator ------------------------------- """

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
            self.__get_string_literal_encoding_id('True'), self.__get_string_literal_encoding_id('False'))

    def visit_IsNot(self, node):
        # TODO: logic can be placed under `object.__eq__` once
        # dynamic dispatching is supported.
        self.__add_instr('objneq', 0, 0)
        self.__add_instr('cldobj',
            self.__get_string_literal_encoding_id('True'), self.__get_string_literal_encoding_id('False'))

    def visit_In(self, node):
        pass

    def visit_NotIn(self, node):
        pass

    """ ------------------------- comprehension ---------------------------- """

    def visit_comprehension(self, node):
        # Do nothing here.
        # Comprehension syntax gets unwrapped in code_transformer.py
        pass

    """ ------------------------- excepthandler ---------------------------- """

    def visit_excepthandler(self, node):
        # Do nothing here. Except handlers are implemented in `visit_TryExcept`.
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
                # This is a kwarg.
                self.__add_instr('getkwarg', self.__get_string_literal_encoding_id(arg.id), 0)
                vector_length1 = len(self.__current_vector())
                self.visit(default)
                self.__add_instr('stobj', self.__get_string_literal_encoding_id(arg.id), 0)
                vector_length2 = len(self.__current_vector())
                length_diff = vector_length2 - vector_length1
                self.__current_vector()[vector_length1 - 1].oprd2 = length_diff
            else:
                # This is an arg. It's handled in `visit_Name()`.
                self.visit(arg)

        # Pull out rest of the args (*args).
        if node.vararg:
            self.__add_instr('getargs', 0, 0)
            self.__add_instr('new', 0, 0)
            self.__add_instr('setval', 0, 0)
            self.__add_instr('stobj', self.__get_string_literal_encoding_id(node.vararg), 0)

        # Pull out rest of the kwargs (**kwarg).
        if node.kwarg:
            self.__add_instr('getkwargs', 0, 0)
            self.__add_instr('new', 0, 0)
            self.__add_instr('setval', 0, 0)
            self.__add_instr('stobj', self.__get_string_literal_encoding_id(node.kwarg), 0)

## -----------------------------------------------------------------------------

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
        '-m',
        '--metadata-file',
        action='store',
        dest='metadata_file',
        help='coreVM metadata file')

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
        sys.exit(-1)

    if not options.metadata_file:
        sys.stderr.write('Info file not specified\n')
        sys.exit(-1)

    if not options.output_file:
        sys.stderr.write('Output file not specified\n')
        sys.exit(-1)

    try:
        generator = BytecodeGenerator(options)

        # Loads the built-in definitions.
        # NOTE: The definition of `bool` has to come before all other types.
        # because it defines the names `True` and `False` that are needed in
        # the logic.

        generator.set_name_mangling_mode(BytecodeGenerator.NameManglingMode.MANGLE_BUILTIN)

        generator.read_from_source('python/src/__builtin__.py')
        generator.read_from_source('python/src/__debug__.py')
        generator.read_from_source('python/src/bool.py')
        generator.read_from_source('python/src/none.py')
        generator.read_from_source('python/src/int.py')
        generator.read_from_source('python/src/float.py')
        generator.read_from_source('python/src/complex.py')
        generator.read_from_source('python/src/str.py')
        generator.read_from_source('python/src/list.py')
        generator.read_from_source('python/src/tuple.py')
        generator.read_from_source('python/src/dict.py')
        generator.read_from_source('python/src/set.py')
        generator.read_from_source('python/src/frozenset.py')
        generator.read_from_source('python/src/slice.py')
        generator.read_from_source('python/src/exceptions.py')

        generator.set_name_mangling_mode(BytecodeGenerator.NameManglingMode.NONE)

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
