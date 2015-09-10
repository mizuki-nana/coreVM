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
CONST_STR_OPEN_BRACKET = __call_cls_builtin(str, '[')
CONST_STR_CLOSE_BRACKET = __call_cls_builtin(str, ']')
CONST_INT_0 = __call_cls_builtin(int, 0)
CONST_INT_1 = __call_cls_builtin(int, 1)

## -----------------------------------------------------------------------------

class list(object):

    def __init__(self, arg):
        """
        ### BEGIN VECTOR ###
        [ldobj, self, 0]
        [ldobj, arg, 0]
        [cpyhndl, 13, 0]
        ### END VECTOR ###
        """

    def append(self, arg):
        """
        ### BEGIN VECTOR ###
        [gethndl2, self, 0]
        [ldobj, arg, 0]
        [putobj, 0, 0]
        [aryapnd, 0, 0]
        [ldobj, self, 0]
        [sethndl, 0, 0]
        ### END VECTOR ###
        """
        return None

    def __len__(self):
        """
        ### BEGIN VECTOR ###
        [gethndl2, self, 0]
        [arylen, 0, 0]
        [new, 0, 0]
        [sethndl, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call_cls_builtin(int, res_)

    def __str__(self):
        size = __call_method_0(self.__len__)
        top_index = __call_method_1(size.__sub__, CONST_INT_1)
        index = __call_cls_builtin(int, 0)
        res = __call_cls_builtin(str, '')
        __call_method_1(res.__add__, CONST_STR_OPEN_BRACKET)

        iterator_ = __call_method_0(self.__iter__)
        try:
            while True:
                item = __call_method_0(iterator_.next)

                __call_method_1(res.__add__, __call_method_0(item.__repr__)) # res += str(item)

                if __call_method_1(index.__lt__, top_index):
                    __call_method_1(res.__add__, CONST_STR_COMMA)

                __call_method_1(index.__iadd__, CONST_INT_1)
        except StopIteration:
            __call_method_1(res.__add__, CONST_STR_CLOSE_BRACKET)

        return res

    def __repr__(self):
        return __call_method_0(self.__str__)

    def __hash__(self):
        raise __call_cls_0(TypeError)

    def __iter__(self):
        return __call_cls_1(listiterator, self)

    def __get_item_by_index(self, i):
        if __call_method_1(i.__gte__, __call_method_0(self.__len__)):
            raise __call_cls_0(IndexError)

        """
        ### BEGIN VECTOR ###
        [gethndl2, self, 0]
        [gethndl2, i, 0]
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

        res_ = __call_cls_1(list, self)

        if step is not None:
            if __call_method_1(step.__eq__, CONST_INT_0):
                raise __call_cls_0(ValueError)

            if __call_method_1(step.__lt__, CONST_INT_0):
                """
                ### BEGIN VECTOR ###
                [ldobj, res_, 0]
                [gethndl, 0, 0]
                [reverse, 0, 0]
                [sethndl, 0, 0]
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
        [ldobj, res_, 0]
        [gethndl, 0, 0]
        [gethndl2, start, 0]
        [gethndl2, stop, 0]
        [slice, 0, 0]
        [sethndl, 0, 0]
        ### END VECTOR ###
        """

        if step is not None:
            """
            ### BEGIN VECTOR ###
            [ldobj, res_, 0]
            [gethndl, 0, 0]
            [gethndl2, step, 0]
            [stride, 0, 0]
            [sethndl, 0, 0]
            ### END VECTOR ###
            """

        return res_

    def __setitem__(self, i, value):
        if __call_method_1(i.__gte__, __call_method_0(self.__len__)):
            raise __call_cls_0(IndexError)

        """
        ### BEGIN VECTOR ###
        [gethndl2, self, 0]
        [gethndl2, i, 0]
        [ldobj, value, 0]
        [putobj, 0, 0]
        [aryput, 0, 0]
        [ldobj, self, 0]
        [sethndl, 0, 0]
        ### END VECTOR ###
        """
        return None

## -----------------------------------------------------------------------------

class listiterator(object):

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
