class int(object):

    def __init__(self, value):
        """
        ### BEGIN VECTOR ###
        [ldobj, value, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [ldobj, self, 0]
        [sethndl, 0, 0]
        ### END VECTOR ###
        """

    def __str__(self, arg_):
        """
        ### BEGIN VECTOR ###
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [repr, 0, 0]
        [new, 0, 0]
        [sethndl, 0, 0]
        [stobj, value, 0]
        [sethndl, 0, 0]
        ### END VECTOR ###
        """
        str_obj = __call(str, value)
        return str_obj

    def __add__(self, value):
        """
        ### BEGIN VECTOR ###
        [ldobj, value, 0]
        [gethndl, 0, 0]
        [pop, 0, 0]
        [ldobj, self, 0]
        [sethndl, 0, 0]
        ### END VECTOR ###
        """
