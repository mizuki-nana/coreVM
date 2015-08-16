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

## -----------------------------------------------------------------------------

def test_subtractions():
    print c2 - c1
    print c3 - c2
    print c1 - c4
    print c4 - c3

## -----------------------------------------------------------------------------

def test_multiplications():
    print c1 * c2
    print c3 * c4
    print c4 * c1
    print c1 * c1
    print c4 * c4

## -----------------------------------------------------------------------------

def test_divisions():
    print c4 / c1

## -----------------------------------------------------------------------------

def test_powers():
    print c1 ** 2
    print c2 ** 3
    print c3 ** 4
    print c4 ** 4

## -----------------------------------------------------------------------------

def test_equalities():
    print c1 == c1
    print c1 == c2
    print c3 != c3
    print c3 != c4

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

## -----------------------------------------------------------------------------

test_repr()
test_additions()
test_subtractions()
test_multiplications()
test_divisions()
test_powers()
test_equalities()
test_positives()
test_negatives()

## -----------------------------------------------------------------------------
