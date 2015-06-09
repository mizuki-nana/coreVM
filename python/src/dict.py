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

class dict(object):

    class __dict_KeyValuePair(object):

        def __init__(self, key, value):
            # TODO: Use Python code after we can do aug-assignment on
            # attributes.
            """
            ### BEGIN VECTOR ###
            [ldobj, self, 0]
            [ldobj, key, 0]
            [setattr, key, 0]
            [ldobj, value, 0]
            [setattr, value, 0]
            ### END VECTOR ###
            """

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

    def keys(self):
        res = __call_cls_builtin(list, [])

        iterator_ = __call_method(__call_method(self.items).__iter__)

        try:
            while True:
                item = __call_method(iterator_.next)
                __call_method(res.append, item.key)
        except StopIteration:
            pass

        return res

    def values(self):
        res = __call_cls_builtin(list, [])

        iterator_ = __call_method(__call_method(self.items).__iter__)

        try:
            while True:
                item = __call_method(iterator_.next)
                __call_method(res.append, item.value)
        except StopIteration:
            pass

        return res

    def items(self):
        # TODO: Convert `__dict_KeyValuePair` instances to tuples.
        """
        ### BEGIN VECTOR ###
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [mapvals, 0, 0]
        [new, 0, 0]
        [sethndl, 0, 0]
        [stobj, items_, 0]
        ### END VECTOR ###
        """
        return __call_cls_builtin(list, items_)

    def __contains__(self, key):
        key_hash = __call_method(key.__hash__)

        """
        ### BEGIN VECTOR ###
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [ldobj, key_hash, 0]
        [gethndl, 0, 0]
        [mapfind, 0, 0]
        [new, 0, 0]
        [sethndl, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call_cls_builtin(bool, res_)

    def __getitem__(self, key):
        if __call_method(self.__contains__, key):
            key_hash = __call_method(key.__hash__)

            """
            ### BEGIN VECTOR ###
            [ldobj, self, 0]
            [gethndl, 0, 0]
            [ldobj, key_hash, 0]
            [gethndl, 0, 0]
            [mapat, 0, 0]
            [getobj, 0, 0]
            [stobj, key_val_pair, 0]
            ### END VECTOR ###
            """
            return key_val_pair.value
        else:
            raise __call_cls(KeyError)

    def __setitem__(self, key, value):
        key_hash = __call_method(key.__hash__)

        if __call_method(self.__contains__, key):
            """
            ### BEGIN VECTOR ###
            [ldobj, self, 0]
            [gethndl, 0, 0]
            [ldobj, key_hash, 0]
            [gethndl, 0, 0]
            [mapat, 0, 0]
            [getobj, 0, 0]
            [stobj, key_val_pair, 0]
            ### END VECTOR ###
            """
            key_val_pair.value = value
        else:
            key_val_pair = __call_cls(__dict_KeyValuePair, key, value)
            """
            ### BEGIN VECTOR ###
            [ldobj, self, 0]
            [gethndl, 0, 0]
            [ldobj, key_hash, 0]
            [gethndl, 0, 0]
            [ldobj, key_val_pair, 0]
            [putobj, 0, 0]
            [mapput, 0, 0]
            [ldobj, self, 0]
            [sethndl, 0, 0]
            ### END VECTOR ###
            """

        return None

    def __str__(self):
        res = __call_cls_builtin(str, '')
        __call_method(res.__add__, __call_cls(str, '{'))

        size = __call_method(self.__len__)
        top_index = __call_method(size.__sub__, 1)
        index = __call_cls_builtin(int, 0)

        iterator_ = __call_method(__call_method(self.items).__iter__)

        try:
            while True:
                item = __call_method(iterator_.next)

                key_object = item.key
                value_object = item.value

                # Containers call `__repr__` on elements.
                __call_method(res.__add__, __call_method(key_object.__repr__))
                __call_method(res.__add__, __call_cls_builtin(str, ': '))
                __call_method(res.__add__, __call_method(value_object.__repr__))

                if __call_method(index.__lt__, top_index):
                    __call_method(res.__add__, __call_cls_builtin(str, ', '))

                index = __call_method(index.__add__, 1)

        except StopIteration:
            __call_method(res.__add__, __call_cls_builtin(str, '}'))

    def __repr__(self):
        return __call_method(self.__str__)

    def __hash__(self):
        raise __call_cls(TypeError)

    def __iter__(self):
        return __call_cls(dict_keyiterator, self)

## -----------------------------------------------------------------------------

class dict_keyiterator(object):

    def __init__(self, iterable):
        self.iterable = iterable
        self.i = __call_cls_builtin(int, 0)
        self.n = __call_method(__call_method(iterable.keys).__len__)

    def next(self):
        if __call_method(self.i.__lt__, self.n):
            key = __call_method(__call_method(self.iterable.keys).__getitem__, self.i)
            __call_method(self.i.__iadd__, __call_cls_builtin(int, 1))
            return key
        else:
            raise __call_cls(StopIteration)

## -----------------------------------------------------------------------------
