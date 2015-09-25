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

def test_pow_without_z():
    print pow(0, 0)
    print pow(0, 2)
    print pow(1, 2)
    print pow(5, 0)
    print pow(2, 10)
    print pow(-2, 10)
    print pow(True, True)
    print pow(True, False)
    print pow(False, True)
    print pow(False, False)

    assert pow(1, -2) == 1.0
    print pow(-5, 0)
    print pow(2, -6)
    print pow(-2, -6)

    print pow(3.142, 2)
    print pow(4.0, -2) == 0.25

    # TODO: [COREVM-367] Make `pow` instruction handle floating-number operands
    # print pow(2, 3.14)

    try:
        print pow(0, -2)
    except ZeroDivisionError:
        print '0.0 cannot be raised to a negative power'

## -----------------------------------------------------------------------------

def test_pow_with_z():
    pow(0, 0, 1)
    pow(1, 2, 3)
    pow(5, 3, 5)
    pow(True, True, True)
    pow(False, True, True)
    pow(True, False, True)

    def test_pow_with_invalid_x_and_y(x, y, z):
        try:
            pow(x, y, z)
        except TypeError:
            print 'Invalid x or y to pow'

    test_pow_with_invalid_x_and_y(0.0, 1, 1)
    test_pow_with_invalid_x_and_y(1, 0.0, 1)
    test_pow_with_invalid_x_and_y(3.14, 2, 1)
    test_pow_with_invalid_x_and_y(2, 3.14, 1)
    test_pow_with_invalid_x_and_y(2, -3, 1)

    def test_pow_with_invalid_z(x, y, z):
        try:
            pow(x, y, z)
        except ValueError:
            print 'Calling pow() with invalid z'

    test_pow_with_invalid_z(0, 0, 0)

    def test_pow_with_invalid_z_and_typerror(x, y, z):
        try:
            pow(x, y, z)
        except TypeError:
            print 'Calling pow() with invalid z'

    test_pow_with_invalid_z_and_typerror(3, 4, 3.14)

## -----------------------------------------------------------------------------

test_pow_without_z()
test_pow_with_z()

## -----------------------------------------------------------------------------
