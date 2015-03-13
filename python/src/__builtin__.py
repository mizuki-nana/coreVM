class type:
    pass


class object:

    def __new__(cls):
        """
        ### BEGIN VECTOR ###
        [new, 0, 0]
        [ldobj, cls, 0]
        [setattr, __class__, 0]
        ### END VECTOR ###
        """

def __call(caller):
    # Need to support *args and **kwargs.
    if caller.__class__ is type:
        return caller.__init__(object.__new__(caller))
    else:
        return caller()
