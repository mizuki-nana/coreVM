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

class tuple(object):

    def __init__(self, arg):
        """
        ### BEGIN VECTOR ###
        [ldobj, self, 0]
        [ldobj, arg, 0]
        [cpyhndl, 13, 0]
        ### END VECTOR ###
        """

    def __len__(self):
        """
        ### BEGIN VECTOR ###
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [arylen, 0, 0]
        [new, 0, 0]
        [sethndl, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call_cls(int, res_)

    def __add__(self, other):
        """
        ### BEGIN VECTOR ###
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [ldobj, other, 0]
        [gethndl, 0, 0]
        [arymrg, 0, 0]
        [new, 0, 0]
        [sethndl, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call_cls(tuple, res_)

    def __str__(self):
        size = __call_method(self.__len__)
        top_index = __call_method(size.__sub__, 1)
        index = __call_cls(int, 0)
        res = __call_cls(str, '')
        __call_method(res.__add__, __call_cls(str, '('))

        iterator_ = __call_method(self.__iter__)
        try:
            while True:
                item = __call_method(iterator_.next)

                __call_method(res.__add__, __call_method(item.__repr__)) # res += str(item)

                if __call_method(index.__lt__, top_index):
                    __call_method(res.__add__, __call_cls(str, ', '))

                __call_method(index.__iadd__, __call_cls(int, 1))
        except StopIteration:
            __call_method(res.__add__, __call_cls(str, ')'))

        return res

    def __repr__(self):
        return __call_method(self.__str__)

    def __hash__(self):
        res = __call_cls(int, 0)

        iterator_ = __call_method(self.__iter__)
        try:
            while True:
                item = __call_method(iterator_.next)
                __call_method(res.__iadd__, __call_method(item.__hash__))
        except StopIteration:
            pass

        return res

    def __iter__(self):
        return __call_cls(tupleiterator, self)

    def __getitem__(self, i):
        if __call_method(i.__gte__, __call_method(self.__len__)):
            raise __call_cls(IndexError)

        """
        ### BEGIN VECTOR ###
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [ldobj, i, 0]
        [gethndl, 0, 0]
        [aryat, 0, 0]
        [getobj, 0, 0]
        [stobj, res, 0]
        ### END VECTOR ###
        """
        return res

## -----------------------------------------------------------------------------

class tupleiterator(object):

    def __init__(self, iterable_):
        self.iterable = iterable_
        self.i = __call_cls(int, 0)
        self.n = __call_method(iterable_.__len__)

    def next(self):
        if __call_method(self.i.__lt__, self.n):
            res = __call_method(self.iterable.__getitem__, self.i)
            __call_method(self.i.__iadd__, __call_cls(int, 1))
            return res
        else:
            raise __call_cls(StopIteration)

## -----------------------------------------------------------------------------
