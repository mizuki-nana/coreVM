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

def test_bin_on_integer_type():
    print bin(0)
    print bin(1)
    print bin(2)
    print bin(5)
    print bin(10)
    print bin(100)

    print bin(-0)
    print bin(-1)
    print bin(-22)
    print bin(-333)
    print bin(-4444)
    print bin(-55555)
    print bin(-666666)
    print bin(-7777777)
    print bin(-88888888)
    print bin(-999999999)

    print bin(True)
    print bin(False)
    print bin(bool(1))
    print bin(bool(0))

## -----------------------------------------------------------------------------

def test_bin_on_type_support_index():
    class MyObject(object):
        def __init__(self, x):
            self.x = int(x)

        def __index__(self):
            return self.x

    print bin(MyObject(0))
    print bin(MyObject(1))
    print bin(MyObject(33))
    print bin(MyObject(-567))
    print bin(MyObject(10000000))

## -----------------------------------------------------------------------------

def test_bin_on_unsupported_types():
    def test_bin_with_typerror(x):
        try:
            bin(x)
        except TypeError:
            print 'Expected TypeError for x'

    test_bin_with_typerror(float(3.1415))
    test_bin_with_typerror(complex(3, 4))
    test_bin_with_typerror(str("Hello"))
    test_bin_with_typerror(list([1, 2, 3]))
    test_bin_with_typerror(tuple([1, 2, 3]))
    test_bin_with_typerror(set({1, 2, 3}))
    test_bin_with_typerror(dict({1: 2, 3: 4}))
    test_bin_with_typerror(None)
    test_bin_with_typerror(slice(1, 2, 3))

## -----------------------------------------------------------------------------

test_bin_on_integer_type()
test_bin_on_type_support_index()
test_bin_on_unsupported_types()

## -----------------------------------------------------------------------------
