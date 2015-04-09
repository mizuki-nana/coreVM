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
        for item in self:
            __call(res.__add__, __call(item.__repr__)) # res += str(item)
            if __call(index.__lt__, top_index):
                __call(res.__add__, __call(str, ', '))
            index = __call(index.__add__, 1)
        __call(res.__add__, __call(str, ']'))
        return res

    def __repr__(self):
        return __call(self.__str__)
