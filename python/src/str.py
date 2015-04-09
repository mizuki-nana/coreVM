class str(object):

    def __init__(self, value):
        """
        ### BEGIN VECTOR ###
        [ldobj, value, 0]
        [gethndl, 0, 0]
        [2str, 0, 0]
        [pop, 0, 0]
        [ldobj, self, 0]
        [sethndl, 0, 0]
        ### END VECTOR ###
        """

    def __str__(self):
        return self

    def __repr__(self):
        res = __call(str, '')
        __call(res.__add__, __call(str, "'"))
        __call(res.__add__, self)
        __call(res.__add__, __call(str, "'"))
        return res

    def __add__(self, other):
        """
        ### BEGIN VECTOR ###
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [ldobj, other, 0]
        [gethndl, 0, 0]
        [strapd, 0, 0]
        [pop, 0, 0]
        [sethndl, 0, 0]
        [rtrn, 0, 0]
        ### END VECTOR ###
        """
