class type:
    pass


class MethodType:
    pass


class object:

    def __new__(cls, arg):
        """
        ### BEGIN VECTOR ###
        [new, 0, 0]
        [ldobj, cls, 0]
        [gethndl, 0, 0]
        [setattr, __class__, 0]
        [setattrs, 0, 0]
        [rsetattrs, im_self, 0]
        ### END VECTOR ###
        """

def __call(caller, arg):
    # Need to support *args and **kwargs.
    if caller.__class__ is type:
        obj = object.__new__(caller, arg)
        caller.__init__(obj, arg)
        return obj
    elif caller.__class__ is MethodType:
        return caller(caller.im_self, arg)
    else:
        return caller(arg)
