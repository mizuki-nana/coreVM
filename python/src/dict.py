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
        for item in __call(self.items):
            __call(res.append, item.key)
        return res

    def values(self):
        res = __call(list, [])
        for item in __call(self.items):
            __call(res.append, item.value)
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

    def __str__(self):
        res = __call(str, '')
        __call(res.__add__, __call(str, '{'))

        size = __call(self.__len__)
        top_index = __call(size.__sub__, 1)
        index = __call(int, 0)

        for item in __call(self.items):
            key_object = item.key
            value_object = item.value

            # Containers call `__repr__` on elements.
            __call(res.__add__, __call(key_object.__repr__))
            __call(res.__add__, __call(str, ': '))
            __call(res.__add__, __call(value_object.__repr__))

            if __call(index.__lt__, top_index):
                __call(res.__add__, __call(str, ', '))
            index = __call(index.__add__, 1)

        __call(res.__add__, __call(str, '}'))
        return res

    def __repr__(self):
        return __call(self.__str__)
