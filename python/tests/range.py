# The MIT License (MIT)

# Copyright (c) 2015 Yanzheng Li

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the 'Software'), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.


## -----------------------------------------------------------------------------

def test_range_with_one_argument():
    print range(0)
    print range(1)
    print range(2)
    print range(5)
    print range(10)
    print range(100)

    print range(-1)
    print range(-2)
    print range(-5)
    print range(-10)
    print range(-100)

    print range(True)
    print range(False)

## -----------------------------------------------------------------------------

def test_range_with_one_invalid_argument():
    def test_range_with_typeerror(x):
        try:
            range(x)
        except TypeError:
            print 'Expected TypeError on calling range() with argument'

    test_range_with_typeerror(None)
    test_range_with_typeerror(3.14)
    test_range_with_typeerror('1')

## -----------------------------------------------------------------------------

def test_range_with_two_arguments():
    print range(0, 0)
    print range(0, 1)
    print range(1, 1)
    print range(1, 2)
    print range(1, 10)
    print range(2, 9)
    print range(0, 10)

    print range(1, 0)
    print range(0, -1)
    print range(100, 99)
    print range(10, 0)

## -----------------------------------------------------------------------------

def test_range_with_two_invalid_arguments():
    def test_range_with_typeerror(x, y):
        try:
            range(x, y)
        except TypeError:
            print 'Expected TypeError on calling range() with argument'

    test_range_with_typeerror(0, None)
    test_range_with_typeerror(None, 1)
    test_range_with_typeerror(None, None)
    test_range_with_typeerror(0, 3.14)
    test_range_with_typeerror(3.14, 0)
    test_range_with_typeerror(3, '14')

## -----------------------------------------------------------------------------

def test_range_with_three_arguments():
    print range(0, 0, 1)
    print range(0, 10, 1)
    print range(0, 10, 2)
    print range(10, 0, -1)
    print range(9, 1, -2)
    print range(-1, -2, -1)

    print range(0, 0, -1)
    print range(0, 10, -1)
    print range(0, 10, -2)

    print range(0, -10, 2)
    print range(9, -1, -2)
    print range(-1, -2, -1)
    print range(-1, -10, -12)

    print range(10, 0, 1)
    print range(9, 1, 2)
    print range(-1, -2, 1)

## -----------------------------------------------------------------------------

def test_range_with_three_invalid_arguments():
    def test_range_with_typeerror(x, y, z):
        try:
            range(x, y, z)
        except TypeError:
            print 'Expected TypeError on calling range() with argument'

    test_range_with_typeerror(0, 0, None)
    test_range_with_typeerror(None, None, 1)
    test_range_with_typeerror(None, None, None)
    test_range_with_typeerror(0, 0, 3.14)
    test_range_with_typeerror(3.14, 0, 1)
    test_range_with_typeerror(3, '14', '1')

    def test_range_with_valueerror(x, y, z):
        try:
            range(x, y, z)
        except ValueError:
            print 'Expected ValueError on calling range() with argument'

    test_range_with_valueerror(0, 0, 0)

## -----------------------------------------------------------------------------

test_range_with_one_argument()
test_range_with_one_invalid_argument()
test_range_with_two_arguments()
test_range_with_two_invalid_arguments()
test_range_with_three_arguments()
test_range_with_three_invalid_arguments()

## -----------------------------------------------------------------------------
