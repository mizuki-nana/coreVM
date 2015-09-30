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

def test_float_representations():
    print 5824.674626
    print float(3.141592)
    print -8.315381
    print float(-3.141592)
    print 3.14e05 == 314000.0
    print 3.14e-05 == 0.000031
    print -3.14e-05 == -0.000031
    print -0.0314e5 == -3140
    print -0.0314e-5 == -0.000000314
    print 123456789012345e-15 == 0.123456789012345

## -----------------------------------------------------------------------------

def test_float_unary_operations():
    # NOTE: We cannot simply do `-9.999999` here because the Python `ast` module
    # treats that as a single number instead of a unary negation operator applied
    # on a floating number.
    # (Python 2.7.6 on Ubuntu 14.04).
    print -(9.999999)
    print +10.123456
    print +(-(-(123.725197)))
    print -(-9.999999)
    print +(-3.141592)
    print -(-(123.123456))

## -----------------------------------------------------------------------------

def test_float_binary_operations():
    print 1.329431 + 5.953167
    print 99.838301 - 99.000123
    print 123.456 * 987.654
    print 999.666333 / 3.00
    print 100.000001 % 33.000000
    print 180.0000 % 3.141593
    print 9.234 ** 4.76
    print int(9.99 // 3.31) # printing precision issue
    print 99.912312 - (-123.333333)
    print -8.888888 / -4.000000

## -----------------------------------------------------------------------------

def test_float_modulo_operations():
    print 180.000 % 3.141592
    print -180.000 % 3.141592
    print 180.00 % -3.141592
    print -180.000 % -3.141592

    print 180.300003 % 3.0
    print -180.300003 % 3.0
    print 180.300003 % -3.0
    print -180.300003 % -3.0

## -----------------------------------------------------------------------------

def test_float_aug_assignments():
    i = 1.34
    i += 3.585739
    print i

    j = 111.555555
    j -= 110.444444
    j -= -1.111212
    print j

    k = 123.554
    k *= 321.649
    print k

    u = 777.777777
    u /= 7
    u /= -1.0
    print u

    v = 639.0
    v %= 5.0
    print v == float(639.0 % 5.0)

    w = 23.444
    w **= 3.59
    print w

    fd = 368.5
    fd //= 5.0
    print fd == int(368 // 5.0)

## -----------------------------------------------------------------------------

def test_float_comparisons():
    if 1.01 == 1.01:
        print 'A fact is a fact.'

    if 1.01 != 1.02:
        print 'Zero tolerance on imprecision here'

    if 1.0 < 1.1:
        print 'Precision is key'

    if 1.123456789 <= 1.123456789:
        print 'Relentless pursuit of high-precision engineering'

    if float(1.0) > float(0.99):
        print 'My statement still holds true'

    if float(0.992) >= float(0.991):
        print 'You know the story'

    if 1.01 != -1.01:
        print 'True'

    if (float(-912.11)) < float(-3.1111):
        print 'Also true'

    if 3.1415 + 6.12741 > 3.1315 * -6.12741:
        print 'This should be true'

## -----------------------------------------------------------------------------

def test_float_nonzero():
    i = 0.0
    print i.__nonzero__()

    i = -0.0
    print i.__nonzero__()

    i = -0.0000000
    print i.__nonzero__()

    i = 1.01
    print i.__nonzero__()

    i = -1.000001
    print i.__nonzero__()

    i = 9.0 / 100000
    print i.__nonzero__()

    i = 9.0 / -100000
    print i.__nonzero__()

## -----------------------------------------------------------------------------

test_float_representations()
test_float_unary_operations()
test_float_binary_operations()
test_float_modulo_operations()
test_float_aug_assignments()
test_float_comparisons()
test_float_nonzero()

## -----------------------------------------------------------------------------
