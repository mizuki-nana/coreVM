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

def test_abs_on_builtin_types():
    # Integers.
    print abs(0)
    print abs(10)
    print abs(-5)

    # Floating points.
    print abs(3.141592)
    print abs(-999.999999)
    print abs(0.0) == 0.0

    # Complex numbers.
    print abs(complex(3, 4)) == 5.0
    print abs(complex(-4.0, 3.0)) == 5.0
    print abs(complex(0, 0)) == 0.0

    # Booleans.
    print abs(True)
    print abs(False)
    print abs(bool(0))

## -----------------------------------------------------------------------------

def test_abs_on_unsupported_types():
    def test_abs_with_typerror(x):
        try:
            abs(x)
        except TypeError:
            print 'Expected TypeError on x'

    test_abs_with_typerror("Hello world")
    test_abs_with_typerror(list([1, 2, 3]))
    test_abs_with_typerror(tuple([1, 2, 3]))
    test_abs_with_typerror(set({1, 2, 3}))
    test_abs_with_typerror(dict({1: 1, 2: 2}))
    test_abs_with_typerror(None)
    test_abs_with_typerror(slice(1, 2, 3))

## -----------------------------------------------------------------------------

test_abs_on_builtin_types()
test_abs_on_unsupported_types()

## -----------------------------------------------------------------------------
