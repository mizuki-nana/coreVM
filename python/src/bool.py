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

class bool(object):

    def __init__(self, value):
        """
        ### BEGIN VECTOR ###
        [getval2, value, 0]
        [truthy, 0, 0]
        [ldobj, self, 0]
        [setval, 0, 0]
        ### END VECTOR ###
        """

    def __str__(self):
        if self:
            return __call_cls_builtin(str, 'True')
        else:
            return __call_cls_builtin(str, 'False')

    def __hash__(self):
        return __call_cls(int, self)

    def __repr__(self):
        return __call_method_0(self.__str__)

    def __nonzero__(self):
        """
        ### BEGIN VECTOR ###
        [getval2, self, 0]
        [bool, 0, 0]
        [neq, 0, 0]
        [cldobj, True, False]
        ### END VECTOR ###
        """

    def __abs__(self):
        """
        ### BEGIN VECTOR ###
        [getval2, self, 0]
        [abs, 0, 0]
        [new, 0, 0]
        [setval, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call_cls_builtin(int, res_)

    def __add__(self, value):
        """
        ### BEGIN VECTOR ###
        [getval2, value, 0]
        [getval2, self, 0]
        [add, 0, 0]
        [new, 0, 0]
        [setval, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call_cls_builtin(bool, res_)

    def __sub__(self, value):
        """
        ### BEGIN VECTOR ###
        [getval2, value, 0]
        [getval2, self, 0]
        [sub, 0, 0]
        [new, 0, 0]
        [setval, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call_cls_builtin(bool, res_)

    def __mul__(self, value):
        """
        ### BEGIN VECTOR ###
        [getval2, value, 0]
        [getval2, self, 0]
        [mul, 0, 0]
        [new, 0, 0]
        [setval, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call_cls_builtin(bool, res_)

    def __div__(self, value):
        """
        ### BEGIN VECTOR ###
        [getval2, value, 0]
        [getval2, self, 0]
        [div, 0, 0]
        [new, 0, 0]
        [setval, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call_cls_builtin(bool, res_)

    def __mod__(self, value):
        """
        ### BEGIN VECTOR ###
        [getval2, value, 0]
        [getval2, self, 0]
        [mod, 0, 0]
        [new, 0, 0]
        [setval, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call_cls_builtin(bool, res_)

    def __pow__(self, value):
        """
        ### BEGIN VECTOR ###
        [getval2, value, 0]
        [getval2, self, 0]
        [pow, 0, 0]
        [new, 0, 0]
        [setval, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call_cls(int, res_)

    def __lshift__(self, value):
        """
        ### BEGIN VECTOR ###
        [getval2, value, 0]
        [getval2, self, 0]
        [bls, 0, 0]
        [new, 0, 0]
        [setval, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call_cls(int, res_)

    def __rshift__(self, value):
        """
        ### BEGIN VECTOR ###
        [getval2, value, 0]
        [getval2, self, 0]
        [brs, 0, 0]
        [new, 0, 0]
        [setval, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call_cls(int, res_)

    def __or__(self, value):
        """
        ### BEGIN VECTOR ###
        [getval2, value, 0]
        [getval2, self, 0]
        [bor, 0, 0]
        [new, 0, 0]
        [setval, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call_cls(int, res_)

    def __xor__(self, value):
        """
        ### BEGIN VECTOR ###
        [getval2, value, 0]
        [getval2, self, 0]
        [bxor, 0, 0]
        [new, 0, 0]
        [setval, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call_cls(int, res_)

    def __and__(self, value):
        """
        ### BEGIN VECTOR ###
        [getval2, value, 0]
        [getval2, self, 0]
        [band, 0, 0]
        [new, 0, 0]
        [setval, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call_cls(int, res_)

    def __floordiv__(self, value):
        """
        ### BEGIN VECTOR ###
        [getval2, value, 0]
        [getval2, self, 0]
        [div, 0, 0]
        [new, 0, 0]
        [setval, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call_cls(int, res_)

    def __iadd__(self, value):
        """
        ### BEGIN VECTOR ###
        [getval2, value, 0]
        [ldobj, self, 0]
        [getval, 0, 0]
        [add, 0, 0]
        [setval, 0, 0]
        ### END VECTOR ###
        """

    def __isub__(self, value):
        """
        ### BEGIN VECTOR ###
        [getval2, value, 0]
        [ldobj, self, 0]
        [getval, 0, 0]
        [sub, 0, 0]
        [setval, 0, 0]
        ### END VECTOR ###
        """

    def __imul__(self, value):
        """
        ### BEGIN VECTOR ###
        [getval2, value, 0]
        [ldobj, self, 0]
        [getval, 0, 0]
        [mul, 0, 0]
        [setval, 0, 0]
        ### END VECTOR ###
        """

    def __idiv__(self, value):
        """
        ### BEGIN VECTOR ###
        [getval2, value, 0]
        [ldobj, self, 0]
        [getval, 0, 0]
        [div, 0, 0]
        [setval, 0, 0]
        ### END VECTOR ###
        """

    def __imod__(self, value):
        """
        ### BEGIN VECTOR ###
        [getval2, value, 0]
        [ldobj, self, 0]
        [getval, 0, 0]
        [mod, 0, 0]
        [setval, 0, 0]
        ### END VECTOR ###
        """

    def __ipow__(self, value):
        """
        ### BEGIN VECTOR ###
        [getval2, value, 0]
        [ldobj, self, 0]
        [getval, 0, 0]
        [pow, 0, 0]
        [setval, 0, 0]
        ### END VECTOR ###
        """

    def __ilshift__(self, value):
        """
        ### BEGIN VECTOR ###
        [getval2, value, 0]
        [ldobj, self, 0]
        [getval, 0, 0]
        [bls, 0, 0]
        [setval, 0, 0]
        ### END VECTOR ###
        """

    def __irshift__(self, value):
        """
        ### BEGIN VECTOR ###
        [getval2, value, 0]
        [ldobj, self, 0]
        [getval, 0, 0]
        [brs, 0, 0]
        [setval, 0, 0]
        ### END VECTOR ###
        """

    def __ior__(self, value):
        """
        ### BEGIN VECTOR ###
        [getval2, value, 0]
        [ldobj, self, 0]
        [getval, 0, 0]
        [bor, 0, 0]
        [setval, 0, 0]
        ### END VECTOR ###
        """

    def __ixor__(self, value):
        """
        ### BEGIN VECTOR ###
        [getval2, value, 0]
        [ldobj, self, 0]
        [getval, 0, 0]
        [bxor, 0, 0]
        [setval, 0, 0]
        ### END VECTOR ###
        """

    def __iand__(self, value):
        """
        ### BEGIN VECTOR ###
        [getval2, value, 0]
        [ldobj, self, 0]
        [getval, 0, 0]
        [band, 0, 0]
        [setval, 0, 0]
        ### END VECTOR ###
        """

    def __ifloordiv__(self, value):
        """
        ### BEGIN VECTOR ###
        [getval2, value, 0]
        [ldobj, self, 0]
        [getval, 0, 0]
        [div, 0, 0]
        [setval, 0, 0]
        ### END VECTOR ###
        """

    def __invert__(self):
        """
        ### BEGIN VECTOR ###
        [getval2, self, 0]
        [2int8, 0, 0]
        [bnot, 0, 0]
        [new, 0, 0]
        [setval, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call_cls(int, res_)

    def __not__(self):
        """
        ### BEGIN VECTOR ###
        [getval2, self, 0]
        [lnot, 0, 0]
        [new, 0, 0]
        [setval, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call_cls_builtin(bool, res_)

    def __pos__(self):
        """
        ### BEGIN VECTOR ###
        [getval2, self, 0]
        [2int8, 0, 0]
        [pos, 0, 0]
        [new, 0, 0]
        [setval, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call_cls_1(int, res_)

    def __neg__(self):
        """
        ### BEGIN VECTOR ###
        [getval2, self, 0]
        [2int8, 0, 0]
        [neg, 0, 0]
        [new, 0, 0]
        [setval, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call_cls_1(int, res_)

    # TODO: Equality methods can be placed under `object` once
    # dynamic dispatching is supported.
    def __eq__(self, other):
        """
        ### BEGIN VECTOR ###
        [getval2, other, 0]
        [getval2, self, 0]
        [eq, 0, 0]
        [new, 0, 0]
        [setval, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call_cls_builtin(bool, res_)

    def __ne__(self, other):
        """
        ### BEGIN VECTOR ###
        [getval2, other, 0]
        [getval2, self, 0]
        [neq, 0, 0]
        [new, 0, 0]
        [setval, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call_cls_builtin(bool, res_)

    def __lt__(self, other):
        """
        ### BEGIN VECTOR ###
        [getval2, other, 0]
        [getval2, self, 0]
        [lt, 0, 0]
        [new, 0, 0]
        [setval, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call_cls_builtin(bool, res_)

    def __lte__(self, other):
        """
        ### BEGIN VECTOR ###
        [getval2, other, 0]
        [getval2, self, 0]
        [lte, 0, 0]
        [new, 0, 0]
        [setval, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call_cls_builtin(bool, res_)

    def __gt__(self, other):
        """
        ### BEGIN VECTOR ###
        [getval2, other, 0]
        [getval2, self, 0]
        [gt, 0, 0]
        [new, 0, 0]
        [setval, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call_cls_builtin(bool, res_)

    def __gte__(self, other):
        """
        ### BEGIN VECTOR ###
        [getval2, other, 0]
        [getval2, self, 0]
        [gte, 0, 0]
        [new, 0, 0]
        [setval, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call_cls_builtin(bool, res_)

    def __cmp__(self, other):
        """
        ### BEGIN VECTOR ###
        [getval2, other, 0]
        [getval2, self, 0]
        [cmp, 0, 0]
        [new, 0, 0]
        [setval, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call_cls_builtin(int, res_)


### True
"""
### BEGIN VECTOR ###
[new, 0, 0]
[ldobj, bool, 0]
[setattr, __class__, 0]
[bool, 1, 0]
[setval, 0, 0]
[stobj, True, 0]
### END VECTOR ###
"""
True = __call_cls(bool, 1)
"""
### BEGIN VECTOR ###
[ldobj, True, 0]
[setflgc, 1, 0]
[setfldel, 1, 0]
[setflmute, 1, 0]
[pop, 0, 0]
### END VECTOR ###
"""


### False
"""
### BEGIN VECTOR ###
[new, 0, 0]
[ldobj, bool, 0]
[setattr, __class__, 0]
[bool, 0, 0]
[setval, 0, 0]
[stobj, False, 0]
### END VECTOR ###
"""
False = __call_cls(bool, 0)
"""
### BEGIN VECTOR ###
[ldobj, False, 0]
[setflgc, 1, 0]
[setfldel, 1, 0]
[setflmute, 1, 0]
[pop, 0, 0]
### END VECTOR ###
"""
