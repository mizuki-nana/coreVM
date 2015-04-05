class NoneType(object):

    def __init__(self):
        """
        ### BEGIN VECTOR ###
        [ldobj, self, 0]
        [bool, 0, 0]
        [sethndl, 0, 0]
        ### END VECTOR ###
        """

    def __str__(self):
        return __call(str, 'None')

    def __not__(self):
        return True


## Global `None` object.
#
# NOTE: In Python it's a syntax error to assign a value to the name `None`,
# so we cannot do `None = __call(NoneType)` here.
#
# TODO: Set the appropriate flags on the global object `None`.
__call(NoneType)
"""
### BEGIN VECTOR ###
[stobj, None, 0]
### END VECTOR ###
"""
