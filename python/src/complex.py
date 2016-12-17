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


## -----------------------------------------------------------------------------

CONST_INT_0 = __call_cls_builtin(int, 0)
CONST_INT_2 = __call_cls_builtin(int, 2)

## -----------------------------------------------------------------------------

class complex(object):

    def __init__(self, real, imag):
        self.real = real
        self.imag = imag

    def __hash__(self):
        real_hash = __call_method_0(self.real.__hash__)
        imag_hash = __call_method_0(self.imag.__hash__)

        PRIME = __call_cls_builtin(int, 12357)

        real_intermediate = __call_method_1(real_hash.__mul__, PRIME)
        imag_intermediate = __call_method_1(imag_hash.__mul__, PRIME)

        return __call_method_1(real_intermediate.__add__, imag_intermediate)

    def __str__(self):
        # Special case if real part is 0, just print imaginary part.
        if __call_method_1(self.real.__eq__, CONST_INT_0):
            res = __call_cls_builtin(str, '')
            __call_method_1(res.__add__, __call_method_0(self.imag.__str__))
            __call_method_1(res.__add__, __call_cls_builtin(str, 'j'))
            return res

        res = __call_cls_builtin(str, '(')

        __call_method_1(res.__add__, __call_method_0(self.real.__str__))

        if __call_method_1(self.imag.__gte__, __call_cls_builtin(int, 0)):
            __call_method_1(res.__add__, __call_cls_builtin(str, '+'))

        __call_method_1(res.__add__, __call_method_0(self.imag.__str__))
        __call_method_1(res.__add__, __call_cls_builtin(str, 'j)'))

        return res

    def __nonzero__(self):
        res_real = __call_method_0(self.real.__nonzero__)
        res_imag = __call_method_0(self.imag.__nonzero__)

        return res_real or res_imag

    def __abs__(self):
        res_real = __call_cls_1(float, self.real)
        res_imag = __call_cls_1(float, self.imag)

        res_real_sqr = __call_method_1(res_real.__pow__, CONST_INT_2)
        res_imag_sqr = __call_method_1(res_imag.__pow__, CONST_INT_2)

        res_sum = __call_method_1(res_real_sqr.__add__, res_imag_sqr)
        res_sqrt = __call_cls_builtin(float, 0)

        """
        ### BEGIN VECTOR ###
        [ldobj, res_sqrt, 0]
        [getval2, res_sum, 0]
        [sqrt, 0, 0]
        [setval, 0, 0]
        ### END VECTOR ###
        """

        return res_sqrt

    def __pos__(self):
        res_real = __call_cls_1(int, self.real)
        res_imag = __call_cls_1(int, self.imag)

        return __call_cls_2(complex, res_real, res_imag)

    def __neg__(self):
        res_real = __call_method_0(self.real.__neg__)
        res_imag = __call_method_0(self.imag.__neg__)

        return __call_cls_2(complex, res_real, res_imag)

    def __add__complex(self, other):
        real = self.real
        imag = self.imag

        res_real = __call_method_1(real.__add__, other.real)
        res_imag = __call_method_1(imag.__add__, other.imag)

        return __call_cls_2(complex, res_real, res_imag)

    def __add__(self, other):
        if other.__class__ is complex:
            return __call_method_1(self.__add__complex, other)
        elif other.__class__ is not int and other.__class__ is not float:
            raise __call_cls_0(TypeError)

        res_real = __call_method_1(self.real.__add__, other)
        res_imag = __call_cls_1(int, self.imag)

        return __call_cls_2(complex, res_real, res_imag)

    def __sub__complex(self, other):
        real = self.real
        imag = self.imag

        res_real = __call_method_1(real.__sub__, other.real)
        res_imag = __call_method_1(imag.__sub__, other.imag)

        return __call_cls_2(complex, res_real, res_imag)

    def __sub__(self, other):
        if other.__class__ is complex:
            return __call_method_1(self.__sub__complex, other)
        elif other.__class__ is not int and other.__class__ is not float:
            raise __call_cls_0(TypeError)

        res_real = __call_method_1(self.real.__sub__, other)
        res_imag = __call_cls_1(int, self.imag)

        return __call_cls_2(complex, res_real, res_imag)

    def __mul__complex(self, other):
        real = self.real
        imag = self.imag

        res_real_1 = __call_method_1(real.__mul__, other.real)
        res_real_2 = __call_method_1(imag.__mul__, other.imag)
        res_real = __call_method_1(
            res_real_1.__add__, __call_method_0(res_real_2.__neg__))

        res_imag_1 = __call_method_1(real.__mul__, other.imag)
        res_imag_2 = __call_method_1(imag.__mul__, other.real)

        res_imag = __call_method_1(res_imag_1.__add__, res_imag_2)

        return __call_cls_2(complex, res_real, res_imag)

    def __mul__(self, other):
        if other.__class__ is complex:
            return __call_method_1(self.__mul__complex, other)
        elif other.__class__ is not int and other.__class__ is not float:
            raise __call_cls_0(TypeError)

        res_real = __call_method_1(self.real.__mul__, other)
        res_imag = __call_method_1(self.imag.__mul__, other)

        return __call_cls_2(complex, res_real, res_imag)

    def __div__complex(self, other):
        numerator = __call_method_1(self.__mul__, __call_method_0(other.conjugate))
        denominator_1 = __call_method_1(other.real.__mul__, other.real)
        denominator_2 = __call_method_1(other.imag.__mul__, other.imag)
        denominator = __call_method_1(denominator_1.__add__, __call_method_0(denominator_2.__abs__))

        numerator_real_float = __call_cls_1(float, numerator.real)
        numerator_imag_float = __call_cls_1(float, numerator.imag)

        res_real = __call_method_1(numerator_real_float.__div__, denominator)
        res_imag = __call_method_1(numerator_imag_float.__div__, denominator)

        return __call_cls_2(complex, res_real, res_imag)

    def __div__(self, other):
        if other.__class__ is complex:
            return __call_method_1(self.__div__complex, other)
        elif other.__class__ is not int and other.__class__ is not float:
            raise __call_cls_0(TypeError)

        res_real = __call_method_1(self.real.__div__, other)
        res_imag = __call_method_1(self.imag.__div__, other)

        return __call_cls_2(complex, res_real, res_imag)

    def __pow__(self, n):
        # TODO: [COREVM-415] Add support for Python complex type exponentiation

        n_ = __call_cls(int, n)
        zero = __call_cls_builtin(int, 0)
        one = __call_cls_builtin(int, 1)

        real = __call_cls_1(int, self.real)
        imag = __call_cls_1(int, self.imag)

        res = __call_cls_2(complex, real, imag)

        while __call_method_1(n_.__gt__, one):
            res = __call_method_1(res.__mul__, self)
            n_ = __call_method_1(n_.__sub__, one)

        return res

    def __eq__(self, other):
        real_eq = __call_method_1(self.real.__eq__, other.real)
        imag_eq = __call_method_1(self.imag.__eq__, other.imag)

        return real_eq and imag_eq

    def __ne__(self, other):
        return __call_method_0(__call_method_1(self.__eq__, other).__not__)

    def __lt__(self, other):
        raise __call_cls_0(TypeError)

    def __lte__(self, other):
        raise __call_cls_0(TypeError)

    def __gt__(self, other):
        raise __call_cls_0(TypeError)

    def __gte__(self, other):
        raise __call_cls_0(TypeError)

    def conjugate(self):
        res_real = __call_cls(int, self.real)
        res_imag = __call_method_0(self.imag.__neg__)

        return __call_cls_2(complex, res_real, res_imag)

## -----------------------------------------------------------------------------
