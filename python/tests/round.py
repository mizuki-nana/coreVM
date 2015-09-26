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

def test_round_without_ndigits():
    assert round(0.0) == 0.0
    assert round(3.1415) == 3.0
    assert round(-3.1415) == -3.0
    assert round(1.0) == 1.0
    assert round(1) == 1.0
    assert round(True) == 1.0
    assert round(False) == 0.0

## -----------------------------------------------------------------------------

def test_round_with_ndigits():
    assert round(0.0, 0) == 0.0
    assert round(3.1415, 2) == 3.14
    assert round(1.0, 2) == 1.0
    assert round(123.1415926, 4) == 123.1416
    assert round(-123.1415926, 4) == -123.1416
    assert round(123.1415926, -4) == 0.0
    assert round(-123.1415926, -4) == 0.0
    assert round(3.14, True) == 3.1
    assert round(3.14, False) == 3.0

## -----------------------------------------------------------------------------

def test_round_with_invalid_operands():
    def test_round_with_typerror(x, y):
        try:
            print round(x, y)
        except TypeError:
            print 'Expected TypeError for round'

    test_round_with_typerror('3.14', 2)
    test_round_with_typerror(3.14, 2.0)
    test_round_with_typerror(3, 2.0)

## -----------------------------------------------------------------------------

test_round_without_ndigits()
test_round_with_ndigits()
test_round_with_invalid_operands()

## -----------------------------------------------------------------------------
