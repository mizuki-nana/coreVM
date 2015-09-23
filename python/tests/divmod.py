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

def test_divmod_on_int_type():
    print divmod(1, 2)
    print divmod(5, 3)
    print divmod(5, 5)
    print divmod(5, 7)
    print divmod(100, 7)
    print divmod(49, -7)

## -----------------------------------------------------------------------------

def test_divmod_on_float_type():
    res = divmod(3.14, 2.1)
    assert res[0] == 1.0 and res[1] == 1.04

    res = divmod(100.0, 10.0)
    assert res[0] == 10.0 and res[1] == 0.0

    res = divmod(100.0, -10.0)
    assert res[0] == -10.0 and res[1] == 0.0

    res = divmod(7.16, 2.45)
    assert res[0] == 2.0 and res[1] == 2.26

    res = divmod(49.0, -7.0)
    assert res[0] == -7.0 and res[1] == 0.0

## -----------------------------------------------------------------------------

def test_divmod_on_mixed_types():
    res = divmod(10.0, 5)
    assert res[0] == 2.0 and res[1] == 0.0

    res = divmod(10.0, 3)
    assert res[0] == 3.0 and res[1] == 1.0

    res = divmod(49, -7.0)
    assert res[0] == -7.0 and res[1] == 0.0

    res = divmod(10, 5.0)
    assert res[0] == 2.0 and res[1] == 0.0

    res = divmod(7, 3.14)
    assert res[0] == 2.0 and abs(res[1] - 0.72) < 0.001

## -----------------------------------------------------------------------------

def test_divmod_on_custom_type():
    class MyObject(object):

        def __init__(self, x):
            self.x = x

        def __divmod__(self, other):
            return divmod(self.x, other)

    res = divmod(MyObject(10), 5.0)
    assert res[0] == 2.0 and res[1] == 0.0

    res = divmod(MyObject(10.0), 5)
    assert res[0] == 2.0 and res[1] == 0.0

## -----------------------------------------------------------------------------

def test_divmod_on_unsupported_types():
    def test_divmod_with_typerror(a, b):
        try:
            divmod(a, b)
        except TypeError:
            print 'Expected TypeError for operands on divmod'

    test_divmod_with_typerror('Hello', 2)
    test_divmod_with_typerror([1], 2)

## -----------------------------------------------------------------------------

test_divmod_on_int_type()
test_divmod_on_float_type()
test_divmod_on_mixed_types()
test_divmod_on_custom_type()
test_divmod_on_unsupported_types()

## -----------------------------------------------------------------------------
