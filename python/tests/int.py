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

def test_int_representations():
    print 1234567890
    print int(987654321)

## -----------------------------------------------------------------------------

def test_int_unary_operations():
    print not 1
    print not 0

    print -1
    print +0
    print -(+123)
    print +(-(+321))

## -----------------------------------------------------------------------------

def test_int_binary_operations():
    print 1 + 2
    print 987654321 - 123456789
    print 567 * 654
    print 101 / 11
    print 110 % 33
    print 2**3
    print 1 << 3
    print 99 >> 2
    print 123 | 321
    print 12345 ^ 67890
    print 10101 & 8563732
    print 5 // 2
    print ~1
    print ~0

## -----------------------------------------------------------------------------

def test_int_aug_assignments():
    i = 1
    i += 3
    print i

    j = 111
    j -= 110
    print j

    k = 123
    k *= 321
    print k

    u = 777
    u /= 7
    print u

    v = 639
    v %= 5
    print v

    w = 23
    w **= 3
    print w

    x = 5000
    x <<= 2
    print x

    y = 9999999
    y >>= 3
    print y

    z = 12345
    z |= 54321
    print z

    zeta = 85638
    zeta ^= 00001
    print zeta

    zelda = 55555
    zelda &= 7654321
    print zelda

    fd = 368
    fd //= 5
    print fd

## -----------------------------------------------------------------------------

def test_int_comparisons():
    if 1 == 1:
        print 'You cannot argue with that'

    if 1 != 2:
        print 'Affirmative'

    if 1 < 2:
        print '1 < 2 is correct'

    if 1 + 1 >= 2:
        print '1 + 1 >= 2 is also true'

    if 100 / 10 <= 100 / 9:
        print 'We can do division :-)'

    if 123 * 321 >= 321 * 123:
        print 'Multiplication is commutative'

## -----------------------------------------------------------------------------

test_int_representations()
test_int_unary_operations()
test_int_binary_operations()
test_int_aug_assignments()
test_int_comparisons()

## -----------------------------------------------------------------------------
