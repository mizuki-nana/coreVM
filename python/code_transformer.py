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

import ast
import optparse
import random
import string
import sys
import traceback


class CodeTransformer(ast.NodeVisitor):

    def __init__(self, options):
        self.options = options
        self.indent_level = 0

    def __indent(self):
        self.indent_level += 1

    def __dedent(self):
        self.indent_level -= 1

    def __indentation(self):
        INDENT = '  '
        return ''.join([INDENT for _ in xrange(self.indent_level)])

    def __get_random_name(self):
        return ''.join(random.choice(string.ascii_letters) for _ in xrange(5))

    def transform(self):
        with open(self.options.input_file, 'r') as fd:
            tree = ast.parse(fd.read())

        transformed_str = self.visit(tree)

        if self.options.debug_mode:
            print 'Writing the following to %s' % self.options.output_file
            print transformed_str

        with open(self.options.output_file, 'w') as fd:
            fd.write(transformed_str)

    """ ------------------------------ mod --------------------------------- """

    def visit_Module(self, node):
        return '\n'.join([self.visit(stmt) for stmt in node.body])

    """ ----------------------------- stmt --------------------------------- """

    def visit_FunctionDef(self, node):
        base_str = '{indentation}def {func_name}({arguments}):\n'

        base_str = base_str.format(
            indentation=self.__indentation(),
            func_name=node.name,
            arguments=self.visit(node.args),
        )

        self.__indent()

        if node.args.vararg:
            base_str += '{vararg} = __call(list, {vararg})\n'.format(vararg=node.args.vararg)

        base_str += '\n'.join([self.visit(stmt) for stmt in node.body])
        base_str += '\n'

        self.__dedent()

        return base_str

    def visit_ClassDef(self, node):
        base_str = '{indentation}class {class_name}:\n'

        base_str = base_str.format(
            indentation=self.__indentation(),
            class_name=node.name,
            body=''.join([self.visit(stmt) for stmt in node.body])
        )

        self.__indent()

        base_str += ''.join([self.visit(stmt) for stmt in node.body])

        self.__dedent()

        return base_str

    def visit_Return(self, node):
        base_str = '{indentation}return'.format(indentation=self.__indentation())

        if node.value:
            base_str += (' ' + self.visit(node.value))

        base_str += '\n'

        return base_str

    def visit_Assign(self, node):
        if isinstance(node.targets[0], ast.Subscript):
            # Special case for assignments on subscripts.
            # TODO: need to handle assignment on multiple targets.
            return '{indentation}__call({target}.__setitem__, {slice}, {value})'.format(
                indentation=self.__indentation(),
                target=self.visit(node.targets[0].value),
                slice=self.visit(node.targets[0].slice),
                value=self.visit(node.value))

        base_str = '{indentation}{targets} = {value}\n'.format(
            indentation=self.__indentation(),
            targets=', '.join([self.visit(target) for target in node.targets]),
            value=self.visit(node.value)
        )

        return base_str

    def visit_AugAssign(self, node):
        # The `ast` module does not have node types for aug-assignment operators.
        # So we have to map them to their corresponding methods.
        OP_AST_TYPE_TO_METHOD_MAP = {
            ast.Add: '__iadd__',
            ast.Sub: '__isub__',
            ast.Mult: '__imul__',
            ast.Div: '__idiv__',
            ast.Mod: '__imod__',
            ast.Pow: '__ipow__',
            ast.LShift: '__ilshift__',
            ast.RShift: '__irshift__',
            ast.BitOr: '__ior__',
            ast.BitXor: '__ixor__',
            ast.BitAnd: '__iand__',
            ast.FloorDiv: '__ifloordiv__',
        }

        return '{indentation}__call({target}.{func}, {value})\n'.format(
            indentation=self.__indentation(),
            target=self.visit(node.target),
            func=OP_AST_TYPE_TO_METHOD_MAP[type(node.op)],
            value=self.visit(node.value))

    def visit_Print(self, node):
        base_str = '{indentation}print'.format(indentation=self.__indentation())

        if node.values:
            base_str += (' ' + '__call(' + self.visit(node.values[0]) + '.__str__' + ')')

        base_str += '\n'

        return base_str

    def visit_For(self, node):
        base_str = "{indentation}try:\n".format(indentation=self.__indentation())

        # Indent lvl 1.
        self.__indent()

        iterator_var_name = self.__get_random_name() + '_iterator_'

        base_str += '{indentation}{iterator_var_name} = __call({iter}.__iter__)\n'.format(
            indentation=self.__indentation(),
            iterator_var_name=iterator_var_name,
            iter=self.visit(node.iter)
        )

        base_str += '{indentation}while True:\n'.format(
            indentation=self.__indentation())

        # Indent lvl 2.
        self.__indent()

        base_str += '{indentation}{target} = __call({iterator_var_name}.next)\n'.format(
            indentation=self.__indentation(),
            target=self.visit(node.target),
            iterator_var_name=iterator_var_name
        )

        for stmt in node.body:
            base_str += (self.visit(stmt) + '\n')

        # Dedent lvl 2.
        self.__dedent()

        # Dedent lvl 1.
        self.__dedent()

        base_str += '{indentation}except StopIteration:\n'.format(
            indentation=self.__indentation())

        # Indent lvl 1.
        self.__indent()

        base_str += '{indentation}pass\n'.format(
            indentation=self.__indentation())

        # Dedent lvl 1.
        self.__dedent()

        return base_str

    def visit_While(self, node):
        base_str = '{indentation}while {test}:\n'.format(
            indentation=self.__indentation(),
            test=self.visit(node.test)
        )

        self.__indent()

        for stmt in node.body:
            base_str += (self.visit(stmt) + '\n')

        self.__dedent()

        if node.orelse:
            base_str += '{indentation}else:\n'.format(
                indentation=self.__indentation())

            self.__indent()

            for stmt in node.orelse:
                base_str += (self.visit(stmt) + '\n')

            self.__dedent()

        return base_str

    def visit_If(self, node):
        base_str = '{indentation}if {expr}:\n'.format(
            indentation=self.__indentation(),
            expr=self.visit(node.test)
        )

        self.__indent()

        for stmt in node.body:
            base_str += (self.visit(stmt) + '\n')

        self.__dedent()

        if node.orelse:
            base_str += '{indentation}else:\n'.format(
                indentation=self.__indentation())

            self.__indent()

            for stmt in node.orelse:
                base_str += (self.visit(stmt) + '\n')

            self.__dedent()

        return base_str

    def visit_Raise(self, node):
        base_str = '{indentation}raise'.format(indentation=self.__indentation())

        if node.type:
            base_str += (' ' + self.visit(node.type))
        else:
            # Raises an instance of `Exception` if target is not specified.
            base_str += (' ' + '__call(Exception)')

        return base_str

    def visit_TryExcept(self, node):
        base_str = '{indentation}try:\n'.format(
            indentation=self.__indentation())

        self.__indent()

        for stmt in node.body:
            base_str += (self.visit(stmt) + '\n')

        self.__dedent()

        for handler in node.handlers:
            base_str += self.visit(handler)

        if node.orelse:
            base_str += '{indentation}else:\n'.format(
                indentation=self.__indentation())

            self.__indent()

            for stmt in node.orelse:
                base_str += self.visit(stmt)

            self.__dedent()

        return base_str

    def visit_Expr(self, node):
        return self.visit(node.value)

    def visit_Pass(self, node):
        return '{indentation}pass'.format(indentation=self.__indentation())

    def visit_Break(self, node):
        return '{indentation}break'.format(indentation=self.__indentation())

    def visit_Continue(self, node):
        return '{indentation}continue'.format(indentation=self.__indentation())

    """ ----------------------------- expr --------------------------------- """

    def visit_Ellipsis(self, node):
        raise NotImplementedError

    def visit_Slice(self, node):
        raise NotImplementedError

    def visit_ExtSlice(self, node):
        raise NotImplementedError

    def visit_Index(self, node):
        return self.visit(node.value)

    def visit_BoolOp(self, node):
        return (' %s ' % self.visit(node.op)).join(
            [self.visit(value) for value in node.values])

    def visit_BinOp(self, node):
        base_str = '__call({lhs}.{func}, {rhs})'

        return base_str.format(
            lhs=self.visit(node.left),
            func=self.visit(node.op),
            rhs=self.visit(node.right)
        )

    def visit_UnaryOp(self, node):
        return '__call({operand}.{op_func})'.format(
            operand=self.visit(node.operand),
            op_func=self.visit(node.op))

    def visit_Lambda(self, node):
        return 'lambda {args}: {body}'.format(
            args=self.visit(node.args),
            body=self.visit(node.body))

    def visit_IfExp(self, node):
        return '({body} if {test} else {orelse})'.format(
            body=self.visit(node.body),
            test=self.visit(node.test),
            orelse=self.visit(node.orelse)
        )

    def visit_Dict(self, node):
        assert len(node.keys) == len(node.values)

        base_str = '__call(dict, '

        base_str += '{\n'

        self.__indent()

        for i in xrange(len(node.keys)):
            base_str += '{indentation}{key}: {value},\n'.format(
                indentation=self.__indentation(),
                key='__call({obj}.__hash__)'.format(
                    obj=self.visit(node.keys[i])
                ),
                value='__call(dict.__dict_KeyValuePair, {key}, {value})'.format(
                    key=self.visit(node.keys[i]),
                    value=self.visit(node.values[i])
                )
            )

        self.__dedent()

        base_str += '}'

        base_str += ')'

        return base_str

    def visit_Set(self, node):
        return '__call(set, {' + ', '.join(
            [
                '__call(set.__set_Item, {expr})'.format(expr=self.visit(expr))
                for expr in node.elts
            ]) + '})'

    def visit_ListComp(self, node):
        # TODO: support multiple generators.
        generator = node.generators[0]

        iterable = self.visit(generator.iter)
        elt = 'lambda {target}: {elt}'.format(
            target=self.visit(generator.target),
            elt=self.visit(node.elt))
        res = '__call(list, [])'
        synthesizer='lambda res, item: __call(res.append, item)'

        predicate='None'

        if generator.ifs:
            # TODO: handle multiple `if`s.
            predicate='lambda {target} : {expr}'.format(
                target=self.visit(generator.target),
                expr=self.visit(generator.ifs[0]))

        return """
               __call(
                   __call(
                       sequence_generator,
                       {iterable},
                       {elt},
                       {res},
                       {synthesizer},
                       {predicate}
                   ).eval
               )
               """.format(
                        iterable=iterable,
                        elt=elt,
                        res=res,
                        synthesizer=synthesizer,
                        predicate=predicate)

    def visit_SetComp(self, node):
        # TODO: support multiple generators.
        generator = node.generators[0]

        iterable = self.visit(generator.iter)
        elt = 'lambda {target}: {elt}'.format(
            target=self.visit(generator.target),
            elt=self.visit(node.elt))
        res = '__call(set, {})'
        synthesizer='lambda res, item: __call(res.add, item)'

        predicate='None'

        if generator.ifs:
            # TODO: handle multiple `if`s.
            predicate='lambda {target} : {expr}'.format(
                target=self.visit(generator.target),
                expr=self.visit(generator.ifs[0]))

        return """
               __call(
                   __call(
                       sequence_generator,
                       {iterable},
                       {elt},
                       {res},
                       {synthesizer},
                       {predicate}
                   ).eval
               )
               """.format(
                        iterable=iterable,
                        elt=elt,
                        res=res,
                        synthesizer=synthesizer,
                        predicate=predicate)

    def visit_DictComp(self, node):
        # TODO: support multiple generators.
        generator = node.generators[0]

        iterable = self.visit(generator.iter)
        res = '__call(dict, {})'
        synthesizer='lambda res, key, value: __call(res.__setitem__, key, value)'
        predicate='None'

        if isinstance(generator.target, ast.Tuple):
            # Dict comprehension where the target is a tuple
            # (i.e. key and value).
            # E.g.
            # new_dict = {k: v * 2 for k, v in old_dict}
            assert len(generator.target.elts) >= 2
            targets = ', '.join([
                self.visit(target) for target in generator.target.elts])
            generator_type = 'dict_pair_generator'
            elt = """
                  lambda {targets}:
                      __call(
                          dict.__dict_KeyValuePair,
                          {key},
                          {value})
                  """.format(
                            targets=targets,
                            key=self.visit(node.key),
                            value=self.visit(node.value))

            if generator.ifs:
                # TODO: handle multiple `if`s.
                predicate='lambda {targets} : {expr}'.format(
                    targets=targets,
                    expr=self.visit(generator.ifs[0]))
        else:
            # Dict comprehension where the target is a single item
            # (i.e. key or value).
            # E.g.
            # new_dict = {k: k * 2 for k in old_dict.keys()}
            generator_type = 'dict_item_generator'
            elt = """
                  lambda {target}:
                      __call(
                          dict.__dict_KeyValuePair,
                          {key},
                          {value})
                  """.format(
                            target=self.visit(generator.target),
                            key=self.visit(node.key),
                            value=self.visit(node.value))

            if generator.ifs:
                # TODO: handle multiple `if`s.
                predicate='lambda {target} : {expr}'.format(
                    target=self.visit(generator.target),
                    expr=self.visit(generator.ifs[0]))

        return """
               __call(
                   __call(
                       {generator_type},
                       {iterable},
                       {elt},
                       {res},
                       {synthesizer},
                       {predicate}
                   ).eval
               )
               """.format(
                        generator_type=generator_type,
                        iterable=iterable,
                        elt=elt,
                        res=res,
                        synthesizer=synthesizer,
                        predicate=predicate)

    def visit_Compare(self, node):
        # Note: Only supports one comparison now.
        op = node.ops[0]

        if any(
            (
                isinstance(op, ast.Eq),
                isinstance(op, ast.NotEq),
                isinstance(op, ast.Lt),
                isinstance(op, ast.LtE),
                isinstance(op, ast.Gt),
                isinstance(op, ast.GtE),
            )
        ):
            base_str='{indentation}__call({left}.{op_func}, {right})'.format(
                indentation=self.__indentation(),
                left=self.visit(node.left),
                op_func=self.visit(op),
                right=self.visit(node.comparators[0])
            )
        elif isinstance(op, ast.In):
            base_str='{indentation}__call({left}.{op_func}, {right})'.format(
                indentation=self.__indentation(),
                left=self.visit(node.comparators[0]),
                op_func=self.visit(op),
                right=self.visit(node.left),
            )
        else:
            # TODO: special support for `is` and `is not` can be removed once
            # dynamic dispatching is supported.
            base_str = '{indentation}({left} {op} {comparator})'.format(
              indentation=self.__indentation(),
              left=self.visit(node.left),
              op=self.visit(op),
              comparator=self.visit(node.comparators[0])
            )

        return base_str

    def visit_Call(self, node):
        base_str = '{indentation}__call({caller}'

        base_str = base_str.format(
            indentation=self.__indentation(),
            caller=self.visit(node.func)
        )

        parts = []

        parts.extend([self.visit(arg) for arg in node.args])
        parts.extend([
            '{keyword}={value}'.format(
                keyword=keyword.arg,
                value=self.visit(keyword.value)
            )
            for keyword in node.keywords
        ])

        if node.starargs:
            parts.append('*' + self.visit(node.starargs))

        if node.kwargs:
            parts.append('**' + self.visit(node.kwargs))

        if parts:
            base_str += (',' + ', '.join(parts))

        base_str += ')'

        return base_str

    def visit_Num(self, node):
        num_type = 'float' if isinstance(node.n, float) else 'int'
        return '__call({num_type}, {n})'.format(
            num_type=num_type, n=str(node.n))

    def visit_Attribute(self, node):
        return '{expr}.{attr}'.format(
            expr=self.visit(node.value),
            attr=str(node.attr)
        )

    def visit_Subscript(self, node):
        return '__call({value}.__getitem__, {slice})'.format(
            value=self.visit(node.value),
            slice=self.visit(node.slice)
        )

    def visit_List(self, node):
        # TODO: disregarding `node.ctx` here.
        return '__call(list, [' + ', '.join([self.visit(expr) for expr in node.elts]) + '])'

    def visit_Tuple(self, node):
        # TODO: disregarding `node.ctx` here.
        return '__call(tuple, (' + ','.join([self.visit(expr) for expr in node.elts]) + '))'

    def visit_Name(self, node):
        return node.id

    def visit_Str(self, node):
        return '__call(str, \"%s\")' % str(node.s)

    """ ---------------------------- boolop -------------------------------- """

    def visit_And(self, node):
        return 'and'

    def visit_Or(self, node):
        return 'or'

    """ --------------------------- operator ------------------------------- """

    def visit_Add(self, node):
        return '__add__'

    def visit_Sub(self, node):
        return '__sub__'

    def visit_Mult(self, node):
        return '__mul__'

    def visit_Div(self, node):
        return '__div__'

    def visit_Mod(self, node):
        return '__mod__'

    def visit_Pow(self, node):
        return '__pow__'

    def visit_LShift(self, node):
        return '__lshift__'

    def visit_RShift(self, node):
        return '__rshift__'

    def visit_BitOr(self, node):
        return '__or__'

    def visit_BitXor(self, node):
        return '__xor__'

    def visit_BitAnd(self, node):
        return '__and__'

    def visit_FloorDiv(self, node):
        return '__floordiv__'

    """ ---------------------------- unaryop ------------------------------- """

    def visit_Invert(self, node):
        return '__invert__'

    def visit_Not(self, node):
        return '__not__'

    def visit_UAdd(self, node):
        return '__pos__'

    def visit_USub(self, node):
        return '__neg__'

    """ ----------------------------- cmpop -------------------------------- """

    def visit_Eq(self, node):
        return '__eq__'

    def visit_NotEq(self, node):
        return '__ne__'

    def visit_Lt(self, node):
        return '__lt__'

    def visit_LtE(self, node):
        return '__lte__'

    def visit_Gt(self, node):
        return '__gt__'

    def visit_GtE(self, node):
        return '__gte__'

    def visit_Is(self, node):
        # TODO: special support for `is` and `is not` can be removed once
        # dynamic dispatching is supported.
        return 'is'

    def visit_IsNot(self, node):
        # TODO: special support for `is` and `is not` can be removed once
        # dynamic dispatching is supported.
        return 'is not'

    def visit_In(self, node):
        return '__contains__'

    def visit_NotIn(self, node):
        pass

    """ ------------------------- comprehension ---------------------------- """

    def visit_comprehension(self, node):
        # Do nothing here.
        # Handled in `visit_ListComp`, `visit_SetComp`, and `visit_DictComp`.
        pass

    """ ------------------------- excepthandler ---------------------------- """

    def visit_ExceptHandler(self, node):
        def exception_type_and_name(node):
            if node.type and node.name:
                return ' {type} as {name}'.format(
                    type=self.visit(node.type), name=self.visit(node.name))
            elif node.type:
                return ' {type}'.format(type=self.visit(node.type))
            elif node.name:
                return ' {name}'.format(name=self.visit(node.name))
            else:
                return ' {type}'.format(type='Exception')

        base_str = '{indentation}except{type_and_name}:\n'.format(
            indentation=self.__indentation(),
            type_and_name=exception_type_and_name(node))

        self.__indent()

        for stmt in node.body:
            base_str += (self.visit(stmt) + '\n')

        self.__dedent()

        return base_str

    """ --------------------------- arguments ------------------------------ """

    def visit_arguments(self, node):
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

        parts = []

        # Iterate through the arguments.
        for arg in node.args:
            default = closest_args_to_defaults.get(arg.col_offset)
            if default:
                parts.append(arg.id + '=' + self.visit(default))
            else:
                parts.append(arg.id)

        if node.vararg:
            parts.append('*' + node.vararg)

        if node.kwarg:
            parts.append('**' + node.kwarg)

        return ', '.join(parts)


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

    if not options.output_file:
        sys.stderr.write('Output file not specified\n')
        return -1

    try:
        transformer = CodeTransformer(options)
        transformer.transform()
    except Exception as ex:
        sys.stderr.write('Failed to process %s\n' % options.input_file)
        sys.stderr.write(str(ex))
        sys.stderr.write('\n')
        if options.debug_mode:
            print traceback.format_exc()
        sys.exit(-1)


if __name__ == '__main__':
    main()
