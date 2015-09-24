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

def test_map_with_single_iterable():
    def double(x):
        return x * 2

    print map(double, [1, 2, 3])

## -----------------------------------------------------------------------------

def test_map_with_multiple_iterables():
    def sum(x, y):
        return x + y

    print map(sum, [], [])
    print map(sum, [1, 2, 3], [4, 5, 6])

    def maybe_sum(x, y):
        if x and y:
            return sum(x, y)

        if x and not y:
            return x

        if y and not x:
            return y

        return None

    print map(maybe_sum, [1, 2], [4, 5, 6])
    print map(maybe_sum, [1, 2, 3, 4], [5])
    print map(maybe_sum, [1, 2, 3, 4, 5], [6, 7, 8, 9])

## -----------------------------------------------------------------------------

def test_map_with_invalid_inputs():
    def mul(x, y):
        return x * y

    class MyObject(object):
        def __init__(self):
            pass

    try:
        map(mul, [1, 2], MyObject())
    except TypeError:
        print 'Expected TypeError'

## -----------------------------------------------------------------------------

test_map_with_single_iterable()
test_map_with_multiple_iterables()
test_map_with_invalid_inputs()

## -----------------------------------------------------------------------------
