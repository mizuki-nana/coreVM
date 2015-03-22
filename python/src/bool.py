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

    def __str__(self, arg_):
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
