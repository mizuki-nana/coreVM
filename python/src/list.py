class list(object):

    def __init__(self, arg):
        """
        ### BEGIN VECTOR ###
        [ldobj, arg, 0]
        [gethndl, 0, 0]
        [2ary, 0, 0]
        [pop, 0, 0]
        [ldobj, self, 0]
        [sethndl, 0, 0]
        ### END VECTOR ###
        """

    def append(self, arg):
        """
        ### BEGIN VECTOR ###
        [ldobj, self, 0]
        [gethndl, 0, 0]
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
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [arylen, 0, 0]
        [new, 0, 0]
        [sethndl, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call(int, res_)

    def __str__(self):
        size = __call(self.__len__)
        top_index = __call(size.__sub__, 1)
        index = __call(int, 0)
        res = __call(str, '')
        __call(res.__add__, __call(str, '['))

        iterator_ = __call(self.__iter__)
        try:
            while True:
                item = __call(iterator_.next)

                __call(res.__add__, __call(item.__repr__)) # res += str(item)

                if __call(index.__lt__, top_index):
                    __call(res.__add__, __call(str, ', '))

                __call(index.__iadd__, __call(int, 1))
        except StopIteration:
            __call(res.__add__, __call(str, ']'))

        return res

    def __repr__(self):
        return __call(self.__str__)

    def __iter__(self):
        return __call(listiterator, self)

    def __getitem__(self, i):
        if __call(i.__gte__, __call(self.__len__)):
            raise __call(IndexError)

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

class listiterator(object):

    def __init__(self, iterable_):
        self.iterable = iterable_
        self.i = __call(int, 0)
        self.n = __call(iterable_.__len__)

    def next(self):
        if __call(self.i.__lt__, self.n):
            res = __call(self.iterable.__getitem__, self.i)
            __call(self.i.__iadd__, __call(int, 1))
            return res
        else:
            raise __call(StopIteration)

## -----------------------------------------------------------------------------
