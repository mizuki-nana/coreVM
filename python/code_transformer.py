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


## -----------------------------------------------------------------------------

class LoopState(object):

    def __init__(self, scope_lvl):
        self.scope_lvl = scope_lvl

## -----------------------------------------------------------------------------

class TryExceptState(object):

    def __init__(self, scope_lvl, loop_lvl, finally_block=None):
        self.scope_lvl = scope_lvl
        self.loop_lvl = loop_lvl
        self.finally_block = finally_block

## -----------------------------------------------------------------------------

class CodeTransformer(ast.NodeVisitor):

    def __init__(self, options):
        self.options = options
        self.indent_level = 0
        self.scope_lvl = 0
        self.current_try_except_lvl = 0
        self.loop_states = []
        self.try_except_states = []

    def __indent(self):
        self.indent_level += 1

    def __dedent(self):
        self.indent_level -= 1

    def __indentation(self):
        INDENT = '  '
        return ''.join([INDENT for _ in xrange(self.indent_level)])

    def __get_random_name(self):
        return ''.join(random.choice(string.ascii_letters) for _ in xrange(5))

    def __push_try_except_state(self, try_except_state):
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

    def __current_loop_state(self):
        assert len(self.loop_states), 'No loop states found'
        return self.loop_states[-1]

    def __push_loop_state(self, state):
        self.loop_states.append(state)

    def __pop_loop_state(self):
        self.loop_states.pop()

    def __get_finally_blocks_for_branch_stmts(self, is_return=False):
        """Returns a set of "finally" blocks for branch statements, namely
        'break', 'continue', and 'return' statements.

        See:
        [COREVM-307] Make break, continue and return statements respect try-finally block

        TODO:
        [COREVM-310] Design and implement new mechanism to make branch statements in Python respect try-finally blocks
        """

        if not self.try_except_states:
            return None

        if not self.loop_states:
            return None

        assert self.scope_lvl == self.__current_try_except_state().scope_lvl, \
            'Branch statement must be under the same scope level as the try-except block'

        assert self.__current_try_except_state().scope_lvl == self.__current_loop_state().scope_lvl, \
            'Try-except block and loop should be under the same scope level'

        finally_blocks = []

        if self.try_except_states:
            target_loop_lvl = self.try_except_states[-1].loop_lvl
            for state in reversed(self.try_except_states):
                # If it's a `return` statement, then catch all the finally blocks.
                if state.finally_block and (is_return or state.loop_lvl == target_loop_lvl):
                    finally_blocks.append(state.finally_block)

        return finally_blocks

    def __add_finally_block_stmts_for_branch_stmt(self, is_return=False):
        base_str = ''

        finally_blocks = self.__get_finally_blocks_for_branch_stmts(is_return=is_return)
        if finally_blocks:
            for finally_block in finally_blocks:
                base_str += ('\n'.join([self.visit(stmt) for stmt in finally_block]))
                base_str += '\n'

        return base_str

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
        # Step in.
        self.scope_lvl += 1

        base_str = '{indentation}def {func_name}({arguments}):\n'

        base_str = base_str.format(
            indentation=self.__indentation(),
            func_name=node.name,
            arguments=self.visit(node.args),
        )

        self.__indent()

        if node.args.vararg:
            base_str += '{indentation}{vararg} = __call_cls_1(tuple, {vararg})\n'.format(
                indentation=self.__indentation(),
                vararg=node.args.vararg)

        if node.args.kwarg:
            base_str += '{indentation}{kwarg} = __call_cls_1(dict, {kwarg})\n'.format(
                indentation=self.__indentation(),
                kwarg=node.args.kwarg)

        base_str += '\n'.join([self.visit(stmt) for stmt in node.body])
        base_str += '\n'

        # Functions return `None` by default.
        base_str += '{indentation}return None\n'.format(
            indentation=self.__indentation())

        self.__dedent()

        # Decorators.
        if node.decorator_list:
            decorated = node.name
            for expr in reversed(node.decorator_list):
                decorated = (self.visit(expr) + '(' + decorated + ')')

            base_str += '{indentation}{name} = {decorated}\n'.format(
                indentation=self.__indentation(),
                name=node.name,
                decorated=decorated)

        # Step out.
        self.scope_lvl -= 1

        return base_str

    def visit_ClassDef(self, node):
        # Step in.
        self.scope_lvl += 1

        base_str = '{indentation}class {class_name}:\n'

        base_str = base_str.format(
            indentation=self.__indentation(),
            class_name=node.name
        )

        self.__indent()

        base_str += '\n'.join([self.visit(stmt) for stmt in node.body])
        base_str += '\n'

        self.__dedent()

        # Decorators.
        if node.decorator_list:
            decorated = node.name
            for expr in reversed(node.decorator_list):
                decorated = (self.visit(expr) + '(' + decorated + ')')

            base_str += '\n{indentation}{name} = {decorated}\n'.format(
                indentation=self.__indentation(),
                name=node.name,
                decorated=decorated)

        # Step out.
        self.scope_lvl -= 1

        return base_str

    def visit_Return(self, node):
        base_str = ''

        # Finally block code.
        base_str += self.__add_finally_block_stmts_for_branch_stmt(is_return=True)

        base_str += '{indentation}return'.format(indentation=self.__indentation())

        if node.value:
            base_str += (' ' + self.visit(node.value))

        base_str += '\n'

        return base_str

    def visit_Delete(self, node):
        """There are 5 types of targets for the `del` keyword in Python, namely:

            1. Names
            2. Attributes
            3. Subscripts
            4. Name lists
            5. Name tuples

        Those are the 5 types of expressions in the language that depends on
        `expr_context`, which supports the `del` context.

        For more info please refer to the Python AST documentation.
        """
        name_targets = []
        attribute_targets = []
        subscript_targets = []
        list_targets = []
        tuple_targets = []

        for target in node.targets:
            if isinstance(target, ast.Name):
                name_targets.append(target)
            elif isinstance(target, ast.Attribute):
                attribute_targets.append(target)
            elif isinstance(target, ast.Subscript):
                subscript_targets.append(target)
            elif isinstance(target, ast.List):
                list_targets.append(target)
            elif isinstance(target, ast.Tuple):
                tuple_targets.append(target)
            else:
                raise Exception('Unexpected type for delete target: %s' % str(type(target)))

        base_str = ''

        if name_targets:
            base_str += '{indentation}del {targets}'.format(
                indentation=self.__indentation(),
                targets=', '.join([self.visit(target) for target in name_targets]))

        if attribute_targets:
            for target in attribute_targets:
                base_str += '{indentation}__call(delattr, {target}, \"{attr}\")\n'.format(
                    indentation=self.__indentation(),
                    target=self.visit(target.value),
                    attr=target.attr)

        if subscript_targets:
            for target in subscript_targets:
                base_str += '{indentation}__call_method_1({target}.__delitem__, {slice})\n'.format(
                    indentation=self.__indentation(),
                    target=self.visit(target.value),
                    slice=self.visit(target.slice))

        if list_targets:
            for list_target in list_targets:
                for target in list_target.elts:
                    if not isinstance(target, ast.Name):
                        raise Exception('Cannot delete expression of type %s' % str(type(target)))

                base_str += '{indentation}del {targets}\n'.format(
                    indentation=self.__indentation(),
                    targets=', '.join([self.visit(target) for target in list_target.elts]))

        if tuple_targets:
            for tuple_target in tuple_targets:
                for target in tuple_target.elts:
                    if not isinstance(target, ast.Name):
                        raise Exception('Cannot delete expression of type %s' % str(type(target)))

                base_str += '{indentation}del {targets}\n'.format(
                    indentation=self.__indentation(),
                    targets=', '.join([self.visit(target) for target in tuple_target.elts]))

        return base_str

    def visit_Assign(self, node):
        if isinstance(node.targets[0], ast.Subscript):
            # Special case for assignments on subscripts.
            # TODO: need to handle assignment on multiple targets.
            return '{indentation}__call_method_2({target}.__setitem__, {slice}, {value})'.format(
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

        return '{indentation}__call_method_1({target}.{func}, {value})\n'.format(
            indentation=self.__indentation(),
            target=self.visit(node.target),
            func=OP_AST_TYPE_TO_METHOD_MAP[type(node.op)],
            value=self.visit(node.value))

    def visit_Print(self, node):
        base_str = ''

        if node.values:
            num = len(node.values)
            for i, value in enumerate(node.values):
                base_str += '{indentation}print {value}\n'.format(
                    indentation=self.__indentation(),
                    value='__call_method_0(' + self.visit(value) + '.__str__' + ')')

                if i + 1 < num:
                    base_str += '{indentation}print {value}\n'.format(
                        indentation=self.__indentation(),
                        value='__call_method_0(__call_cls_builtin(str, " ").__str__)')

            base_str += '{indentation}print {value}\n'.format(
                indentation=self.__indentation(),
                value='__call_method_0(__call_cls_builtin(str, "\\n").__str__)')
        else:
            base_str += '{indentation}print {value}\n'.format(
                indentation=self.__indentation(),
                value='__call_method_0(__call_cls_builtin(str, "\\n").__str__)')

        return base_str

    def visit_For(self, node):
        self.__push_loop_state(LoopState(self.scope_lvl))

        base_str = "{indentation}try:\n".format(indentation=self.__indentation())

        # Indent lvl 1.
        self.__indent()

        iterator_var_name = self.__get_random_name() + '_iterator_'

        base_str += '{indentation}{iterator_var_name} = __call_method_0({iter}.__iter__)\n'.format(
            indentation=self.__indentation(),
            iterator_var_name=iterator_var_name,
            iter=self.visit(node.iter)
        )

        base_str += '{indentation}while True:\n'.format(
            indentation=self.__indentation())

        # Indent lvl 2.
        self.__indent()

        base_str += '{indentation}{target} = __call_method_0({iterator_var_name}.next)\n'.format(
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

        # NOTE: Statements under the `else` part of the syntax get executed only
        # when the for-loop terminates normally (i.e. not abruptly by
        # `break` statements).
        #
        # Reference:
        #   http://psung.blogspot.com/2007/12/for-else-in-python.html
        #
        # Therefore, since normal termination of for-loops raise instances
        # of `StopIteration`'s, which get caught here, and `break` statements
        # cause jumps to outside the entire loop, we can execute the `else`
        # statements here.
        if node.orelse:
            for stmt in node.orelse:
                base_str += (self.visit(stmt) + '\n')
        else:
            base_str += '{indentation}pass\n'.format(
                indentation=self.__indentation())

        # Dedent lvl 1.
        self.__dedent()

        self.__pop_loop_state()

        return base_str

    def visit_While(self, node):
        self.__push_loop_state(LoopState(self.scope_lvl))

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

        self.__pop_loop_state()

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

    def visit_With(self, node):
        body_name = self.__get_random_name()
        base_str = '{indentation}def {func_name}({args}):\n'.format(
            indentation=self.__indentation(),
            func_name=body_name,
            args=self.visit(node.optional_vars) if node.optional_vars else '')

        self.__indent()

        for stmt in node.body:
            base_str += self.visit(stmt)

        self.__dedent()

        base_str += '\n'

        base_str += """{indentation}__call_method_0(
                        __call_cls_2(
                            with_stmt_runner,
                            {manager},
                            {body}
                        ).run
                    )""".format(
                            indentation=self.__indentation(),
                            manager=self.visit(node.context_expr),
                            body=body_name)

        return base_str

    def visit_Raise(self, node):
        base_str = '{indentation}raise'.format(indentation=self.__indentation())

        if node.type:
            base_str += (' ' + self.visit(node.type))
        else:
            # Raises an instance of `Exception` if target is not specified.
            base_str += (' ' + '__call_cls_0(Exception)')

        return base_str

    def visit_TryExcept(self, node):
        # Step in.
        if self.current_try_except_lvl == self.__total_try_except_lvl():
            self.__push_try_except_state(TryExceptState(self.scope_lvl, len(self.loop_states)))

        self.current_try_except_lvl += 1

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

        # Step out.
        if not self.__current_try_except_state().finally_block:
            self.__pop_try_except_state()

        return base_str

    def visit_TryFinally(self, node):
        self.__push_try_except_state(
            TryExceptState(self.scope_lvl, len(self.loop_states), node.finalbody))

        base_str = ''

        for stmt in node.body:
            base_str += self.visit(stmt)

        base_str += '{indentation}finally:\n'.format(
            indentation=self.__indentation())

        self.__indent()

        for stmt in node.finalbody:
            base_str += self.visit(stmt)

        self.__dedent()

        self.__pop_try_except_state()

        return base_str

    def visit_Assert(self, node):
        base_str = '{indentation}if __call(__call_cls_1(bool, {test}).__not__):\n'.format(
            indentation=self.__indentation(),
            test=self.visit(node.test))

        self.__indent()

        if node.msg:
            base_str += '{indentation}raise __call_cls_1(AssertionError, {msg})'.format(
                indentation=self.__indentation(),
                msg=self.visit(node.msg))
        else:
            base_str += '{indentation}raise __call_cls_0(AssertionError)'.format(
                indentation=self.__indentation())

        self.__dedent()

        return base_str

    def visit_Global(self, node):
        assert node.names

        return '{indentation}global {identifiers}\n'.format(
            indentation=self.__indentation(), identifiers=', '.join([name for name in node.names]))

    def visit_Expr(self, node):
        return self.visit(node.value)

    def visit_Pass(self, node):
        return '{indentation}pass'.format(indentation=self.__indentation())

    def visit_Break(self, node):
        base_str = ''

        base_str += self.__add_finally_block_stmts_for_branch_stmt()
        base_str += '{indentation}break'.format(indentation=self.__indentation())

        return base_str

    def visit_Continue(self, node):
        base_str = ''

        base_str += self.__add_finally_block_stmts_for_branch_stmt()
        base_str += '{indentation}continue'.format(indentation=self.__indentation())

        return base_str

    """ ----------------------------- expr --------------------------------- """

    def visit_Ellipsis(self, node):
        raise NotImplementedError

    def visit_Slice(self, node):
        return '__call_cls_3(slice, {start}, {stop}, {step})'.format(
            start=self.visit(node.lower) if node.lower else 'None',
            stop=self.visit(node.upper) if node.upper else 'None',
            step=self.visit(node.step) if node.step else 'None')

    def visit_ExtSlice(self, node):
        raise NotImplementedError

    def visit_Index(self, node):
        return self.visit(node.value)

    def visit_BoolOp(self, node):
        return '(' + (' %s ' % self.visit(node.op)).join(
            [self.visit(value) for value in node.values]) + ')'

    def visit_BinOp(self, node):
        base_str = '__call_method_1({lhs}.{func}, {rhs})'

        return base_str.format(
            lhs=self.visit(node.left),
            func=self.visit(node.op),
            rhs=self.visit(node.right)
        )

    def visit_UnaryOp(self, node):
        return '__call_method_0({operand}.{op_func})'.format(
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

        base_str = '__call_cls_builtin(dict, '

        base_str += '{\n'

        self.__indent()

        for i in xrange(len(node.keys)):
            base_str += '{indentation}{key}: {value},\n'.format(
                indentation=self.__indentation(),
                key='__call_method_0({obj}.__hash__)'.format(
                    obj=self.visit(node.keys[i])
                ),
                value='__call_cls_2(dict.__dict_KeyValuePair, {key}, {value})'.format(
                    key=self.visit(node.keys[i]),
                    value=self.visit(node.values[i])
                )
            )

        self.__dedent()

        base_str += '}'

        base_str += ')'

        return base_str

    def visit_Set(self, node):
        return '__call_cls_builtin(set, {' + ', '.join(
            [
                '__call_cls_1(set.__set_Item, {expr})'.format(expr=self.visit(expr))
                for expr in node.elts
            ]) + '})'

    def __visit_generator(self, generator, callee):
        if generator.ifs:
            return '__call_cls(comprehension, {callee}, predicate={predicate})'.format(
                callee=callee,
                predicate='lambda {target}: {expr}'.format(
                    target=self.visit(generator.target),
                    expr=' and '.join([self.visit(if_expr) for if_expr in generator.ifs]))
                )
        else:
            return '__call_cls_1(comprehension, {callee})'.format(callee=callee)

    def visit_ListComp(self, node):
        generator = node.generators[0]

        elt = 'lambda {target}: {elt}'.format(
            target=self.visit(generator.target),
            elt=self.visit(node.elt))
        res = '__call_cls_builtin(list, [])'
        synthesizer='lambda res, item: __call_method_1(res.append, item)'

        base_str = '__call_cls_3(generator, {elt}, {res}, {synthesizer})'.format(
                        elt=elt,
                        res=res,
                        synthesizer=synthesizer)

        for generator in node.generators:
            base_str = self.__visit_generator(generator, base_str)

        iterable = self.visit(node.generators[-1].iter)

        base_str = '__call_method_1({base_str}.eval, {iterable})'.format(
            base_str=base_str,
            iterable=iterable)

        return base_str

    def visit_SetComp(self, node):
        generator = node.generators[0]

        elt = 'lambda {target}: {elt}'.format(
            target=self.visit(generator.target),
            elt=self.visit(node.elt))
        res = '__call_cls_builtin(set, {})'
        synthesizer='lambda res, item: __call_method_1(res.add, item)'

        base_str = '__call_cls_3(generator, {elt}, {res}, {synthesizer})'.format(
                        elt=elt,
                        res=res,
                        synthesizer=synthesizer)

        for generator in node.generators:
            base_str = self.__visit_generator(generator, base_str)

        iterable = self.visit(node.generators[-1].iter)

        base_str = '__call_method_1({base_str}.eval, {iterable})'.format(
            base_str=base_str,
            iterable=iterable)

        return base_str

    def visit_DictComp(self, node):
        generator = node.generators[0]

        if isinstance(generator.target, ast.Tuple):
            # Dict comprehension where the target is a tuple
            # (i.e. key and value).
            # E.g.
            # new_dict = {k: v * 2 for k, v in old_dict}
            assert len(generator.target.elts) >= 2
            targets = ', '.join([
                self.visit(target) for target in generator.target.elts])
            elt = 'lambda {targets}: __call_cls_2(dict.__dict_KeyValuePair, {key}, {value})'.format(
                        targets=targets,
                        key=self.visit(node.key),
                        value=self.visit(node.value))
        else:
            # Dict comprehension where the target is a single item
            # (i.e. key or value).
            # E.g.
            # new_dict = {k: k * 2 for k in old_dict.keys()}
            generator_type = 'dict_item_generator'
            elt = 'lambda {target}: __call_cls_2(dict.__dict_KeyValuePair, {key}, {value})'.format(
                        target=self.visit(generator.target),
                        key=self.visit(node.key),
                        value=self.visit(node.value))

        res = '__call_cls_builtin(dict, {})'
        synthesizer='lambda res, pair: __call_method_2(res.__setitem__, pair.key, pair.value)'

        base_str = '__call_cls_3(generator, {elt}, {res}, {synthesizer})'.format(
                        elt=elt,
                        res=res,
                        synthesizer=synthesizer)

        for generator in node.generators:
            base_str = self.__visit_generator(generator, base_str)

        iterable = self.visit(node.generators[-1].iter)

        base_str = '__call_method_1({base_str}.eval, {iterable})'.format(
            base_str=base_str,
            iterable=iterable)

        return base_str

    def __generate_compare_with_str(self, left, op, right):
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
            return '__call_method_1({left}.{op_func}, {right})'.format(
                left=left,
                op_func=self.visit(op),
                right=self.visit(right)
            )
        elif isinstance(op, ast.In):
            base_str='__call_method_1({left}.{op_func}, {right})'.format(
                left=self.visit(right),
                op_func=self.visit(op),
                right=left,
            )
        else:
            # TODO: special support for `is` and `is not` can be removed once
            # dynamic dispatching is supported.
            return '({left} {op} {right})'.format(
              left=left,
              op=self.visit(op),
              right=self.visit(right)
            )

    def __generate_compare(self, left, op, right):
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
            return '__call_method_1({left}.{op_func}, {right})'.format(
                indentation=self.__indentation(),
                left=self.visit(left),
                op_func=self.visit(op),
                right=self.visit(right)
            )
        elif isinstance(op, ast.In):
            return '__call_method_1({left}.{op_func}, {right})'.format(
                indentation=self.__indentation(),
                left=self.visit(right),
                op_func=self.visit(op),
                right=self.visit(left),
            )
        else:
            # TODO: special support for `is` and `is not` can be removed once
            # dynamic dispatching is supported.
            return '({left} {op} {right})'.format(
              indentation=self.__indentation(),
              left=self.visit(left),
              op=self.visit(op),
              right=self.visit(right)
            )

    def visit_Compare(self, node):
        """Comparison expressions.

        Need to be able to distinguish between single and chained comparisons.

        Reference:
          https://docs.python.org/3/reference/expressions.html#comparisons
        """
        assert len(node.ops) == len(node.comparators)

        if len(node.ops) == 1:
            # Single comparison.
            # In this case, the two operands in the expression are denoted by
            # `node.left` and `node.comparators[0]`.
            return self.__generate_compare(node.left, node.ops[0], node.comparators[0])

        # Chained comparisons.
        #
        # In this case, all the operands in the expression are encapsulated in
        # `node.comparators`.

        comparators = [node.left]
        comparators.extend(node.comparators)
        res = ''
        counter = 0

        for i in xrange(len(node.ops)):
            if counter != 1:
                counter += 1
                continue

            assert i % 2 == 1
            assert counter == 1

            if not res:
                left = self.__generate_compare(comparators[i-1], node.ops[i-1], comparators[i])
            else:
                left = self.__generate_compare_with_str(res, node.ops[i-1], comparators[i])

            right = self.__generate_compare(comparators[i], node.ops[i], comparators[i+1])

            res = '(({left}) and ({right}))'.format(
                left=left, right=right)

            counter = 0

        return res

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
        base_str = '__call_cls_builtin({num_type}, {n})'.format(
            num_type=num_type, n=str(abs(node.n)))

        if node.n < 0:
            base_str = '__call_method_0({n}.__neg__)'.format(
                n=base_str)

        return base_str

    def visit_Attribute(self, node):
        return '{expr}.{attr}'.format(
            expr=self.visit(node.value),
            attr=str(node.attr)
        )

    def visit_Subscript(self, node):
        return '{indentation}__call_method_1({value}.__getitem__, {slice})'.format(
            indentation=self.__indentation(),
            value=self.visit(node.value),
            slice=self.visit(node.slice)
        )

    def visit_List(self, node):
        # TODO: disregarding `node.ctx` here.
        return '__call_cls_builtin(list, [' + ', '.join([self.visit(expr) for expr in node.elts]) + '])'

    def visit_Tuple(self, node):
        # TODO: disregarding `node.ctx` here.
        return '__call_cls_builtin(tuple, (' + ','.join([self.visit(expr) for expr in node.elts]) + '))'

    def visit_Name(self, node):
        return node.id

    def visit_Str(self, node):
        """
        Python escape characters:
        http://www.codecodex.com/wiki/Escape_sequences_and_escape_characters#Python
        """
        s = str(node.s)
        s = s.replace('\n', '\\n')
        s = s.replace('\t', '\\t')
        s = s.replace('\r', '\\r')
        s = s.replace('\b', '\\b')
        s = s.replace('\"', '\\"')
        s = s.replace("\'", "\\'")
        s = s.replace('\?', '\\?')
        return '__call_cls_builtin(str, \"%s\")' % s

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
