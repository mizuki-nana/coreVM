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
