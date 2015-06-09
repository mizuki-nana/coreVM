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

class set(object):

    class __set_Item(object):

        def __init__(self, value):
            self.value = value
            self.hash = __call_method(value.__hash__)

    def __init__(self, arg):
        """
        ### BEGIN VECTOR ###
        [ldobj, self, 0]
        [ldobj, arg, 0]
        [cpyhndl, 14, 0]
        ### END VECTOR ###
        """

    def __len__(self):
        """
        ### BEGIN VECTOR ###
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [maplen, 0, 0]
        [new, 0, 0]
        [sethndl, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call_cls_builtin(int, res_)

    def __repr__(self):
        return __call_method(self.__str__)

    def __iter__(self):
        return __call_cls(setiterator, self)

    def __hash__(self):
        raise __call_cls(TypeError)

    def __contains__(self, value):
        item = __call_cls(set.__set_Item, value)
        value_hash = item.hash

        """
        ### BEGIN VECTOR ###
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [ldobj, value_hash, 0]
        [gethndl, 0, 0]
        [mapfind, 0, 0]
        [new, 0, 0]
        [sethndl, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call_cls(bool, res_)

    def __str__(self):
        res = __call_cls_builtin(str, 'set([')

        size = __call_method(self.__len__)
        top_index = __call_method(size.__sub__, 1)
        index = __call_cls_builtin(int, 0)

        iterator_ = __call_method(self.__iter__)

        try:
            while True:
                item = __call_method(iterator_.next)

                __call_method(res.__add__, __call_method(item.__repr__))

                if __call_method(index.__lt__, top_index):
                    __call_method(res.__add__, __call_cls_builtin(str, ', '))

                index = __call_method(index.__add__, 1)

        except StopIteration:
            __call_method(res.__add__, __call_cls_builtin(str, '])'))

        return res

    def __eq__(self, other):
        """
        ### BEGIN VECTOR ###
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [mapkeys, 0, 0]
        [ldobj, other, 0]
        [gethndl, 0, 0]
        [mapkeys, 0, 0]
        [eq, 0, 0]
        [new, 0, 0]
        [sethndl, 0, 0]
        [stobj, res, 0]
        ### END VECTOR ###
        """
        return __call_cls(bool, res)

    def __ne__(self, other):
        return __call_method(__call_method(self.__eq__, other).__not__)

    def __sub__(self, other):
        return __call_method(self.difference, other)

    def __or__(self, other):
        return __call_method(self.union, other)

    def __and__(self, other):
        return __call_method(self.intersection, other)

    def __xor__(self, other):
        return __call_method(self.symmetric_difference, other)

    def add(self, value):
        item = __call_cls(set.__set_Item, value)

        """
        ### BEGIN VECTOR ###
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [ldobj, item, 0]
        [getattr, hash, 0]
        [gethndl, 0, 0]
        [ldobj, value, 0]
        [putobj, 0, 0]
        [mapput, 0, 0]
        [ldobj, self, 0]
        [sethndl, 0, 0]
        ### END VECTOR ###
        """

    def clear(self):
        """
        ### BEGIN VECTOR ###
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [mapclr, 0, 0]
        [ldobj, self, 0]
        [sethndl, 0, 0]
        ### END VECTOR ###
        """

    def remove(self, value):
        if __call_method(self.__contains__, value):
            value_hash = __call_method(value.__hash__)
            """
            ### BEGIN VECTOR ###
            [ldobj, self, 0]
            [gethndl, 0, 0]
            [ldobj, value_hash, 0]
            [gethndl, 0, 0]
            [mapers, 0, 0]
            [ldobj, self, 0]
            [sethndl, 0, 0]
            ### END VECTOR ###
            """
            return None
        else:
            raise __call_cls(KeyError)

    def discard(self, value):
        try:
            __call_method(self.remove, value)
        except KeyError:
            pass

    def update(self, other):
        """
        ### BEGIN VECTOR ###
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [ldobj, other, 0]
        [gethndl, 0, 0]
        [mapmrg, 0, 0]
        [ldobj, self, 0]
        [sethndl, 0, 0]
        ### END VECTOR ###
        """

    def issubset(self, other):
        iterator_ = __call_method(self.__iter__)

        res = True

        try:
            while True:
                value = __call_method(iterator_.next)

                res = __call_method(other.__contains__, value)
                if __call_method(res.__not__):
                    return False
        except StopIteration:
            pass

        return res

    def issuperset(self, other):
        return __call_method(other.issubset, self)

    def union(self, other):
        res = __call_cls(set, {})

        iterator_ = __call_method(self.__iter__)

        try:
            while True:
                value = __call_method(iterator_.next)

                __call_method(res.add, value)
        except StopIteration:
            pass

        iterator_ = __call_method(other.__iter__)

        try:
            while True:
                value = __call_method(iterator_.next)

                __call_method(res.add, value)
        except StopIteration:
            pass

        return res

    def intersection(self, other):
        res = __call_cls(set, {})

        iterator_ = __call_method(self.__iter__)

        try:
            while True:
                value = __call_method(iterator_.next)

                if __call_method(other.__contains__, value):
                    __call_method(res.add, value)
        except StopIteration:
            pass

        return res

    def intersection_update(self, other):
        iterator_ = __call_method(self.__iter__)

        try:
            while True:
                value = __call_method(iterator_.next)

                in_other = __call_method(other.__contains__, value)
                not_in_other = __call_method(in_other.__not__)
                if not_in_other:
                    __call_method(self.remove, value)
        except StopIteration:
            pass

    def difference(self, other):
        res = __call_cls_builtin(set, {})

        iterator_ = __call_method(self.__iter__)

        try:
            while True:
                value = __call_method(iterator_.next)

                __call_method(res.add, value)
        except StopIteration:
            pass

        iterator_ = __call_method(other.__iter__)

        try:
            while True:
                value = __call_method(iterator_.next)

                __call_method(res.discard, value)
        except StopIteration:
            pass

        return res

    def difference_update(self, other):
        iterator_ = __call_method(other.__iter__)

        try:
            while True:
                value = __call_method(iterator_.next)

                __call_method(self.discard, value)
        except StopIteration:
            pass

    def symmetric_difference(self, other):
        res = __call_cls_builtin(set, {})

        iterator_ = __call_method(self.__iter__)

        try:
            while True:
                value = __call_method(iterator_.next)

                in_other = __call_method(other.__contains__, value)
                not_in_other = __call_method(in_other.__not__)
                if not_in_other:
                    __call_method(res.add, value)
        except StopIteration:
            pass

        iterator_ = __call_method(other.__iter__)

        try:
            while True:
                value = __call_method(iterator_.next)

                in_self = __call_method(self.__contains__, value)
                not_in_self = __call_method(in_self.__not__)
                if not_in_self:
                    __call_method(res.add, value)
        except StopIteration:
            pass

        return res

    def symmetric_difference_update(self, other):
        new_set = __call_method(self.symmetric_difference, other)

        """
        ### BEGIN VECTOR ###
        [ldobj, new_set, 0]
        [gethndl, 0, 0]
        [ldobj, self, 0]
        [sethndl, 0, 0]
        ### END VECTOR ###
        """

## -----------------------------------------------------------------------------

class setiterator(object):

    def __init__(self, iterable_):
        """
        ### BEGIN VECTOR ###
        [ldobj, iterable_, 0]
        [gethndl, 0, 0]
        [mapvals, 0, 0]
        [new, 0, 0]
        [sethndl, 0, 0]
        [stobj, items_, 0]
        ### END VECTOR ###
        """
        self.items = __call_cls_builtin(list, items_)
        self.iterable = iterable_
        self.i = __call_cls_builtin(int, 0)
        self.n = __call_method(self.items.__len__)

    def next(self):
        if __call_method(self.i.__lt__, self.n):
            res = __call_method(self.items.__getitem__, self.i)
            __call_method(self.i.__iadd__, __call_cls_builtin(int, 1))
            return res
        else:
            raise __call_cls(StopIteration)

## -----------------------------------------------------------------------------
