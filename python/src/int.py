class int(object):

    def __init__(self):
        """
        ### BEGIN VECTOR ###
        [uint32, 0, 0]
        [sethndl, 0, 0]
        ### END VECTOR ###
        """

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
