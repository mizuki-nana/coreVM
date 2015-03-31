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
        [putobj, 0, 0]
        [ldobj, arg, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [aryapnd, 0, 0]
        [sethndl, 0, 0]
        ### END VECTOR ###
        """

    def __str__(self):
        res = __call(str, '')
        __call(res.__add__, __call(str, '['))
        for item in self:
            __call(res.__add__, __call(item.__str__)) # res += str(item)
            __call(res.__add__, __call(str, ', '))
        __call(res.__add__, __call(str, ']'))
        return res
