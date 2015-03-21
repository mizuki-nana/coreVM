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

    def __str__(self, arg_):
        return self
