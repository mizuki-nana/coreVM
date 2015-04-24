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
        [ldobj, arg, 0]
        [gethndl, 0, 0]
        [2map, 0, 0]
        [pop, 0, 0]
        [ldobj, self, 0]
        [sethndl, 0, 0]
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
        return __call(int, res_)

    def keys(self):
        res = __call(list, [])

        iterator_ = __call(__call(self.items).__iter__)

        try:
            while True:
                item = __call(iterator_.next)
                __call(res.append, item.key)
        except StopIteration:
            pass

        return res

    def values(self):
        res = __call(list, [])

        iterator_ = __call(__call(self.items).__iter__)

        try:
            while True:
                item = __call(iterator_.next)
                __call(res.append, item.value)
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
        return __call(list, items_)

    def __contains__(self, key):
        key_hash = __call(key.__hash__)

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
        return __call(bool, res_)

    def __getitem__(self, key):
        if __call(self.__contains__, key):
            key_hash = __call(key.__hash__)

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
            raise __call(KeyError)

    def __str__(self):
        res = __call(str, '')
        __call(res.__add__, __call(str, '{'))

        size = __call(self.__len__)
        top_index = __call(size.__sub__, 1)
        index = __call(int, 0)

        iterator_ = __call(__call(self.items).__iter__)

        try:
            while True:
                item = __call(iterator_.next)

                key_object = item.key
                value_object = item.value

                # Containers call `__repr__` on elements.
                __call(res.__add__, __call(key_object.__repr__))
                __call(res.__add__, __call(str, ': '))
                __call(res.__add__, __call(value_object.__repr__))

                if __call(index.__lt__, top_index):
                    __call(res.__add__, __call(str, ', '))

                index = __call(index.__add__, 1)

        except StopIteration:
            __call(res.__add__, __call(str, '}'))

    def __repr__(self):
        return __call(self.__str__)

    def __hash__(self):
        raise __call(TypeError)

    def __iter__(self):
        return __call(dict_keyiterator, self)

## -----------------------------------------------------------------------------

class dict_keyiterator(object):

    def __init__(self, iterable):
        self.iterable = iterable
        self.i = __call(int, 0)
        self.n = __call(__call(iterable.keys).__len__)

    def next(self):
        if __call(self.i.__lt__, self.n):
            key = __call(__call(self.iterable.keys).__getitem__, self.i)
            __call(self.i.__iadd__, __call(int, 1))
            return key
        else:
            raise __call(StopIteration)

## -----------------------------------------------------------------------------
