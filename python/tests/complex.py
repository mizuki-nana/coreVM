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

c1 = complex(1, 2)
c2 = complex(5, -3)
c3 = complex(-6, 7)
c4 = complex(-2, -4)

## -----------------------------------------------------------------------------

def test_imaginary_number_literals():
    print 5j
    print -3j
    print 0j
    print 100j

## -----------------------------------------------------------------------------

def test_repr():
    print c1
    print c2
    print c3
    print c4

## -----------------------------------------------------------------------------

def test_additions():
    print c1 + c2
    print c2 + c3
    print c3 + c4
    print c4 + c1
    print c1 + 2
    print c2 + 100
    print c3 - 5j

    try:
        print c1 + 'Hello'
    except TypeError:
        print "TypeError: unsupported operand type(s) for +: 'complex' and 'str'"

## -----------------------------------------------------------------------------

def test_subtractions():
    print c2 - c1
    print c3 - c2
    print c1 - c4
    print c4 - c3
    print c1 - 2
    print c3 - -10
    print 3j - c4

    try:
        print c1  - 'Hello'
    except TypeError:
        print "TypeError: unsupported operand type(s) for +: 'complex' and 'str'"

## -----------------------------------------------------------------------------

def test_multiplications():
    print c1 * c2
    print c3 * c4
    print c4 * c1
    print c1 * c1
    print c4 * c4
    print c2 * 10
    print c3 * -5
    print c3 * 3j

    try:
        print c1 * 'Hello'
    except TypeError:
        print "TypeError: unsupported operand type(s) for *: 'complex' and 'str'"

## -----------------------------------------------------------------------------

def test_divisions():
    def verify(expr, actual_result):
        print abs(expr.real - actual_result.real) < 0.0001
        print abs(expr.imag - actual_result.imag) < 0.0001

    verify(c4 / c1, complex(-2, 0))
    verify(complex(10, -5) / complex(5, 0), complex(2, -1))
    verify(complex(10, -5) / complex(5, 1), complex(1.730769, -1.346154))
    verify(5j / 1j, complex(5, 0))
    print c4 / -2

    try:
        print c1 / 'Hello'
    except TypeError:
        print "TypeError: unsupported operand type(s) for /: 'complex' and 'str'"

## -----------------------------------------------------------------------------

def test_powers():
    print c1 ** 2
    print c2 ** 3
    print c3 ** 4
    print c4 ** 4
    print 4j ** 4

## -----------------------------------------------------------------------------

def test_equalities():
    print c1 == c1
    print c1 == c2
    print c3 != c3
    print c3 != c4
    print 5j == 5j

## -----------------------------------------------------------------------------

def test_positives():
    print +c1
    print +c2
    print +c3
    print +c4

## -----------------------------------------------------------------------------

def test_negatives():
    print -c1
    print -c2
    print -c3
    print -c4
    print +5j

## -----------------------------------------------------------------------------

def test_nonzero():
    c = complex(0, 0)
    print c.__nonzero__()

    c = complex(-1, 0)
    print c.__nonzero__()

    c = complex(0, 3.14)
    print c.__nonzero__()

    c = complex(-99, -3.141526)
    print c.__nonzero__()

    c = complex(-99, -999)
    print c.__nonzero__()

    c = -5j
    print c.__nonzero__()

## ----------------------------------------------------------------------------=

def test_comparison_operators():
    def __test_comparison_with_typerror(func):
        try:
            func()
        except TypeError:
            print 'no ordering relation is defined for complex numbers'

    __test_comparison_with_typerror(lambda: c1 < c1)
    __test_comparison_with_typerror(lambda: c1 < c2)
    __test_comparison_with_typerror(lambda: c1 < 2)

    __test_comparison_with_typerror(lambda: c1 <= c1)
    __test_comparison_with_typerror(lambda: c1 <= c2)
    __test_comparison_with_typerror(lambda: c1 <= 2)

    __test_comparison_with_typerror(lambda: c1 > c1)
    __test_comparison_with_typerror(lambda: c1 > c2)
    __test_comparison_with_typerror(lambda: c1 > 2)

    __test_comparison_with_typerror(lambda: c1 >= c1)
    __test_comparison_with_typerror(lambda: c1 >= c2)
    __test_comparison_with_typerror(lambda: c1 >= 2)

    __test_comparison_with_typerror(lambda: 5j < 6j)
    __test_comparison_with_typerror(lambda: -5j > 6j)

## -----------------------------------------------------------------------------

test_imaginary_number_literals()
test_repr()
test_additions()
test_subtractions()
test_multiplications()
test_divisions()
test_powers()
test_equalities()
test_positives()
test_negatives()
test_nonzero()
test_comparison_operators()

## -----------------------------------------------------------------------------
