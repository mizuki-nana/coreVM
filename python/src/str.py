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

class str(object):

    def __init__(self, value):
        """
        ### BEGIN VECTOR ###
        [ldobj, self, 0]
        [ldobj, value, 0]
        [cpyhndl, 12, 0]
        ### END VECTOR ###
        """

    def __str__(self):
        return self

    def __repr__(self):
        res = __call_cls(str, '')
        __call_method(res.__add__, __call_cls(str, "'"))
        __call_method(res.__add__, self)
        __call_method(res.__add__, __call_cls(str, "'"))
        return res

    def __hash__(self):
        """
        ### BEGIN VECTOR ###
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [hash, 0, 0]
        [new, 0, 0]
        [sethndl, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call_cls(int, res_)

    def __add__(self, other):
        """
        ### BEGIN VECTOR ###
        [ldobj, self, 0]
        [gethndl, 0, 0]
        [ldobj, other, 0]
        [gethndl, 0, 0]
        [strapd, 0, 0]
        [pop, 0, 0]
        [sethndl, 0, 0]
        [rtrn, 0, 0]
        ### END VECTOR ###
        """
