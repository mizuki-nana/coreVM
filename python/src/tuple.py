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

## -----------------------------------------------------------------------------

CONST_STR_COMMA = __call_cls_builtin(str, ', ')
CONST_STR_OPEN_PAREN  = __call_cls_builtin(str, '(')
CONST_STR_CLOSE_PAREN  = __call_cls_builtin(str, ')')
CONST_INT_0 = __call_cls_builtin(int, 0)
CONST_INT_1 = __call_cls_builtin(int, 1)

## -----------------------------------------------------------------------------

class tuple(object):

    def __init__(self, arg):
        """
        ### BEGIN VECTOR ###
        [ldobj, self, 0]
        [ldobj, arg, 0]
        [cpyval, 13, 0]
        ### END VECTOR ###
        """

    def __len__(self):
        """
        ### BEGIN VECTOR ###
        [getval2, self, 0]
        [arylen, 0, 0]
        [new, 0, 0]
        [setval, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call_cls_1(int, res_)

    def __add__(self, other):
        """
        ### BEGIN VECTOR ###
        [getval2, other, 0]
        [getval2, self, 0]
        [arymrg, 0, 0]
        [new, 0, 0]
        [setval, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call_cls_builtin(tuple, res_)

    def __str__(self):
        size = __call_method_0(self.__len__)
        top_index = __call_method_1(size.__sub__, 1)
        index = __call_cls_builtin(int, 0)
        res = __call_cls_builtin(str, '')
        __call_method_1(res.__add__, CONST_STR_OPEN_PAREN)

        iterator_ = __call_method_0(self.__iter__)
        try:
            while True:
                item = __call_method_0(iterator_.next)

                __call_method_1(res.__add__, __call_method_0(item.__repr__)) # res += str(item)

                if __call_method_1(top_index.__eq__, CONST_INT_0):
                    __call_method_1(res.__add__, ',')
                elif __call_method_1(index.__lt__, top_index):
                    __call_method_1(res.__add__, CONST_STR_COMMA)

                __call_method_1(index.__iadd__, CONST_INT_1)
        except StopIteration:
            __call_method_1(res.__add__, CONST_STR_CLOSE_PAREN)

        return res

    def __repr__(self):
        return __call_method_0(self.__str__)

    def __hash__(self):
        res = __call_cls_builtin(int, 0)

        iterator_ = __call_method_0(self.__iter__)
        try:
            while True:
                item = __call_method_0(iterator_.next)
                __call_method_1(res.__iadd__, __call_method_0(item.__hash__))
        except StopIteration:
            pass

        return res

    def __iter__(self):
        return __call_cls_1(tupleiterator, self)

    def __get_item_by_index(self, i):
        if __call_method_1(i.__gte__, __call_method_0(self.__len__)):
            raise __call_cls_0(IndexError)

        """
        ### BEGIN VECTOR ###
        [getval2, i, 0]
        [getval2, self, 0]
        [aryat, 0, 0]
        [getobj, 0, 0]
        ### END VECTOR ###
        """

    def __getitem__(self, i):
        # TODO;[COREVM-312] Consolidate sequence slicing logic in Python
        if i.__class__ is int:
            return __call_method_1(self.__get_item_by_index, i)

        # slicing...
        size = __call_method_0(self.__len__)
        start = i.start
        stop = i.stop
        step = i.step

        res_ = __call_cls_1(tuple, self)

        if step is not None:
            if __call_method_1(step.__eq__, CONST_INT_0):
                raise __call_cls_0(ValueError)

            if __call_method_1(step.__lt__, CONST_INT_0):
                """
                ### BEGIN VECTOR ###
                [ldobj, res_, 0]
                [getval, 0, 0]
                [reverse, 0, 0]
                [setval, 0, 0]
                ### END VECTOR ###
                """
                if start is not None:
                    start = __call_method_1(size.__sub__, start)

                    # Special case in slicing when `step` is negative and
                    # only one value of `start` and `stop` is specified.
                    if stop is None and __call_method_1(start.__gt__, CONST_INT_0):
                        start = __call_method_1(start.__sub__, CONST_INT_1)

                if stop is not None:
                    stop = __call_method_1(size.__sub__, stop)

                    # Special case in slicing when `step` is negative and
                    # only one value of `start` and `stop` is specified.
                    if start is None and __call_method_1(stop.__gt__, CONST_INT_0):
                        stop = __call_method_1(stop.__sub__, CONST_INT_1)

                step = __call_method_0(step.__neg__)

        if start is None:
            start = __call_cls_builtin(int, CONST_INT_0)

        if stop is None:
            stop = size

        """
        ### BEGIN VECTOR ###
        [getval2, stop, 0]
        [getval2, start, 0]
        [ldobj, res_, 0]
        [getval, 0, 0]
        [slice, 0, 0]
        [setval, 0, 0]
        ### END VECTOR ###
        """

        if step is not None:
            """
            ### BEGIN VECTOR ###
            [getval2, step, 0]
            [ldobj, res_, 0]
            [getval, 0, 0]
            [stride, 0, 0]
            [setval, 0, 0]
            ### END VECTOR ###
            """

        return res_

## -----------------------------------------------------------------------------

class tupleiterator(object):

    def __init__(self, iterable_):
        self.iterable = iterable_
        self.i = __call_cls_builtin(int, 0)
        self.n = __call_method_0(iterable_.__len__)

    def next(self):
        if __call_method_1(self.i.__lt__, self.n):
            res = __call_method_1(self.iterable.__getitem__, self.i)
            __call_method_1(self.i.__iadd__, CONST_INT_1)
            return res
        else:
            raise __call_cls_0(StopIteration)

## -----------------------------------------------------------------------------
