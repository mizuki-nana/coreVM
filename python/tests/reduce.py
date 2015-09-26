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

def test_reduce_without_initializer():
    product = lambda x, y: x * y

    print reduce(product, [1])
    print reduce(product, [1, 2, 3, 4, 5])
    print reduce(product, [1, 2, 3, -2, -1, -3, 0])

    try:
        reduce(product, [])
    except TypeError:
        print 'Expected TypeError for reduce on empty iterable'

## -----------------------------------------------------------------------------

def test_reduce_with_initializer():
    greatest = lambda x, y: x if x >= y else y

    print reduce(greatest, [], 1)
    print reduce(greatest, [1], 1)
    print reduce(greatest, [2], 1)
    print reduce(greatest, [2], 3)
    print reduce(greatest, [1, 2, 3], 0)
    print reduce(greatest, [1, 2, 3], 1)
    print reduce(greatest, [1, 2, 3], 3)
    print reduce(greatest, [1, 2, 3], 5)
    print reduce(greatest, [1, 2, 3], -5)

## -----------------------------------------------------------------------------

test_reduce_without_initializer()
test_reduce_with_initializer()

## -----------------------------------------------------------------------------
