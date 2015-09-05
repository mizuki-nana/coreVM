# The MIT License (MIT)

# Copyright (c) 2015 Yanzheng Li

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

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
        return __call_cls_builtin(str, 'None')

    def __repr__(self):
        return __call_method_0(self.__str__)

    def __hash__(self):
        # Python 2.7.6 (default, Mar 22 2014, 22:59:56)
        # [GCC 4.8.2] on linux2
        # The version of CPython above returns integer `598041`.
        # We simply follow suit here.
        return __call_cls_builtin(int, 598041)

    def __not__(self):
        return True

    def __eq__(self, other):
        """
        ### BEGIN VECTOR ###
        [ldobj, other, 0]
        [ldobj, self, 0]
        [objeq, 0, 0]
        [cldobj, True, False]
        ### END VECTOR ###
        """

    def __ne__(self, other):
        """
        ### BEGIN VECTOR ###
        [ldobj, other, 0]
        [ldobj, self, 0]
        [objeq, 0, 0]
        [cldobj, False, True]
        ### END VECTOR ###
        """

## -----------------------------------------------------------------------------

## Global `None` object.
#
# NOTE: In Python it's a syntax error to assign a value to the name `None`,
# so we cannot do `None = __call(NoneType)` here.
#
# TODO: Set the appropriate flags on the global object `None`.
__call_cls_0(NoneType)
"""
### BEGIN VECTOR ###
[setflgc, 1, 0]
[setfldel, 1, 0]
[setflmute, 1, 0]
[stobj, None, 0]
### END VECTOR ###
"""

## -----------------------------------------------------------------------------
