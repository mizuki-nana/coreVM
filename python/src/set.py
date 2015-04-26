class set(object):

    class __set_Item(object):

        def __init__(self, value):
            self.value = value
            self.hash = __call(value.__hash__)

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

    def __repr__(self):
        return __call(self.__str__)

    def __iter__(self):
        return __call(setiterator, self)

    def __hash__(self):
        raise __call(TypeError)

    def __contains__(self, value):
        item = __call(set.__set_Item, value)
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
        return __call(bool, res_)

    def __str__(self):
        res = __call(str, 'set([')

        size = __call(self.__len__)
        top_index = __call(size.__sub__, 1)
        index = __call(int, 0)

        iterator_ = __call(self.__iter__)

        try:
            while True:
                item = __call(iterator_.next)

                __call(res.__add__, __call(item.__repr__))

                if __call(index.__lt__, top_index):
                    __call(res.__add__, __call(str, ', '))

                index = __call(index.__add__, 1)

        except StopIteration:
            __call(res.__add__, __call(str, '])'))

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
        return __call(bool, res)

    def __ne__(self, other):
        return __call(__call(self.__eq__, other).__not__)

    def __sub__(self, other):
        return __call(self.difference, other)

    def __or__(self, other):
        return __call(self.union, other)

    def __and__(self, other):
        return __call(self.intersection, other)

    def __xor__(self, other):
        return __call(self.symmetric_difference, other)

    def add(self, value):
        item = __call(set.__set_Item, value)

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
        if __call(self.__contains__, value):
            value_hash = __call(value.__hash__)
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
            raise __call(KeyError)

    def discard(self, value):
        try:
            __call(self.remove, value)
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
        iterator_ = __call(self.__iter__)

        res = True

        try:
            while True:
                value = __call(iterator_.next)

                res = __call(other.__contains__, value)
                if __call(res.__not__):
                    return False
        except StopIteration:
            pass

        return res

    def issuperset(self, other):
        return __call(other.issubset, self)

    def union(self, other):
        res = __call(set, {})

        iterator_ = __call(self.__iter__)

        try:
            while True:
                value = __call(iterator_.next)

                __call(res.add, value)
        except StopIteration:
            pass

        iterator_ = __call(other.__iter__)

        try:
            while True:
                value = __call(iterator_.next)

                __call(res.add, value)
        except StopIteration:
            pass

        return res

    def intersection(self, other):
        res = __call(set, {})

        iterator_ = __call(self.__iter__)

        try:
            while True:
                value = __call(iterator_.next)

                if __call(other.__contains__, value):
                    __call(res.add, value)
        except StopIteration:
            pass

        return res

    def intersection_update(self, other):
        iterator_ = __call(self.__iter__)

        try:
            while True:
                value = __call(iterator_.next)

                in_other = __call(other.__contains__, value)
                not_in_other = __call(in_other.__not__)
                if not_in_other:
                    __call(self.remove, value)
        except StopIteration:
            pass

    def difference(self, other):
        res = __call(set, {})

        iterator_ = __call(self.__iter__)

        try:
            while True:
                value = __call(iterator_.next)

                __call(res.add, value)
        except StopIteration:
            pass

        iterator_ = __call(other.__iter__)

        try:
            while True:
                value = __call(iterator_.next)

                __call(res.discard, value)
        except StopIteration:
            pass

        return res

    def difference_update(self, other):
        iterator_ = __call(other.__iter__)

        try:
            while True:
                value = __call(iterator_.next)

                __call(self.discard, value)
        except StopIteration:
            pass

    def symmetric_difference(self, other):
        res = __call(set, {})

        iterator_ = __call(self.__iter__)

        try:
            while True:
                value = __call(iterator_.next)

                in_other = __call(other.__contains__, value)
                not_in_other = __call(in_other.__not__)
                if not_in_other:
                    __call(res.add, value)
        except StopIteration:
            pass

        iterator_ = __call(other.__iter__)

        try:
            while True:
                value = __call(iterator_.next)

                in_self = __call(self.__contains__, value)
                not_in_self = __call(in_self.__not__)
                if not_in_self:
                    __call(res.add, value)
        except StopIteration:
            pass

        return res

    def symmetric_difference_update(self, other):
        new_set = __call(self.symmetric_difference, other)

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
        self.items = __call(list, items_)
        self.iterable = iterable_
        self.i = __call(int, 0)
        self.n = __call(self.items.__len__)

    def next(self):
        if __call(self.i.__lt__, self.n):
            res = __call(self.items.__getitem__, self.i)
            __call(self.i.__iadd__, __call(int, 1))
            return res
        else:
            raise __call(StopIteration)

## -----------------------------------------------------------------------------
