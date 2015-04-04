class bool(object):

    def __init__(self, value):
        """
        ### BEGIN VECTOR ###
        [ldobj, value, 0]
        [gethndl, 0, 0]
        [2bool, 0, 0]
        [pop, 0, 0]
        [ldobj, self, 0]
        [sethndl, 0, 0]
        ### END VECTOR ###
        """

    def __str__(self):
        if self:
            return __call(str, 'True')
        else:
            return __call(str, 'False')

    def __add__(self, value):
        """
        ### BEGIN VECTOR ###
        [ldobj, value, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [add, 0, 0]
        [new, 0, 0]
        [sethndl, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call(bool, res_)

    def __sub__(self, value):
        """
        ### BEGIN VECTOR ###
        [ldobj, value, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [sub, 0, 0]
        [new, 0, 0]
        [sethndl, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call(bool, res_)

    def __mul__(self, value):
        """
        ### BEGIN VECTOR ###
        [ldobj, value, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [mul, 0, 0]
        [new, 0, 0]
        [sethndl, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call(bool, res_)

    def __div__(self, value):
        """
        ### BEGIN VECTOR ###
        [ldobj, value, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [div, 0, 0]
        [new, 0, 0]
        [sethndl, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call(bool, res_)

    def __mod__(self, value):
        """
        ### BEGIN VECTOR ###
        [ldobj, value, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [mod, 0, 0]
        [new, 0, 0]
        [sethndl, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call(bool, res_)

    def __pow__(self, value):
        """
        ### BEGIN VECTOR ###
        [ldobj, value, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [pow, 0, 0]
        [new, 0, 0]
        [sethndl, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call(int, res_)

    def __lshift__(self, value):
        """
        ### BEGIN VECTOR ###
        [ldobj, value, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [bls, 0, 0]
        [new, 0, 0]
        [sethndl, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call(int, res_)

    def __rshift__(self, value):
        """
        ### BEGIN VECTOR ###
        [ldobj, value, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [brs, 0, 0]
        [new, 0, 0]
        [sethndl, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call(int, res_)

    def __or__(self, value):
        """
        ### BEGIN VECTOR ###
        [ldobj, value, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [bor, 0, 0]
        [new, 0, 0]
        [sethndl, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call(int, res_)

    def __xor__(self, value):
        """
        ### BEGIN VECTOR ###
        [ldobj, value, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [bxor, 0, 0]
        [new, 0, 0]
        [sethndl, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call(int, res_)

    def __and__(self, value):
        """
        ### BEGIN VECTOR ###
        [ldobj, value, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [band, 0, 0]
        [new, 0, 0]
        [sethndl, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call(int, res_)

    def __floordiv__(self, value):
        """
        ### BEGIN VECTOR ###
        [ldobj, value, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [div, 0, 0]
        [new, 0, 0]
        [sethndl, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call(int, res_)

    def __invert__(self):
        """
        ### BEGIN VECTOR ###
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [2int8, 0, 0]
        [bnot, 0, 0]
        [new, 0, 0]
        [sethndl, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call(int, res_)

    def __not__(self):
        """
        ### BEGIN VECTOR ###
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [lnot, 0, 0]
        [new, 0, 0]
        [sethndl, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call(bool, res_)

    def __pos__(self):
        """
        ### BEGIN VECTOR ###
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [2int8, 0, 0]
        [pos, 0, 0]
        [new, 0, 0]
        [sethndl, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call(int, res_)

    def __neg__(self):
        """
        ### BEGIN VECTOR ###
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [2int8, 0, 0]
        [neg, 0, 0]
        [new, 0, 0]
        [sethndl, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call(int, res_)

    # TODO: Equality methods can be placed under `object` once
    # dynamic dispatching is supported.
    def __eq__(self, other):
        """
        ### BEGIN VECTOR ###
        [ldobj, other, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [eq, 0, 0]
        [new, 0, 0]
        [sethndl, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call(bool, res_)

    def __neq__(self, other):
        """
        ### BEGIN VECTOR ###
        [ldobj, other, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [neq, 0, 0]
        [new, 0, 0]
        [sethndl, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call(bool, res_)

    def __lt__(self, other):
        """
        ### BEGIN VECTOR ###
        [ldobj, other, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [lt, 0, 0]
        [new, 0, 0]
        [sethndl, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call(bool, res_)

    def __lte__(self, other):
        """
        ### BEGIN VECTOR ###
        [ldobj, other, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [lte, 0, 0]
        [new, 0, 0]
        [sethndl, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call(bool, res_)

    def __gt__(self, other):
        """
        ### BEGIN VECTOR ###
        [ldobj, other, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [gt, 0, 0]
        [new, 0, 0]
        [sethndl, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call(bool, res_)

    def __gte__(self, other):
        """
        ### BEGIN VECTOR ###
        [ldobj, other, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [gte, 0, 0]
        [new, 0, 0]
        [sethndl, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call(bool, res_)


### True
"""
### BEGIN VECTOR ###
[new, 0, 0]
[ldobj, bool, 0]
[setattr, __class__, 0]
[bool, 1, 0]
[sethndl, 0, 0]
[stobj, True, 0]
### END VECTOR ###
"""
True = __call(bool, 1)


### False
"""
### BEGIN VECTOR ###
[new, 0, 0]
[ldobj, bool, 0]
[setattr, __class__, 0]
[bool, 0, 0]
[sethndl, 0, 0]
[stobj, False, 0]
### END VECTOR ###
"""

False = __call(bool, 0)
