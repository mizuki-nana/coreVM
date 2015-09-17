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

def test_boolean_representations():
    print True
    print False
    print bool(1)
    print bool(0)

## -----------------------------------------------------------------------------

def test_boolean_unary_operators():
    # Python returns int type.
    print ~True
    print ~bool(0)

    print not True
    print not bool(0)

    print -True
    print +False
    print -(+True)
    print +(-(+bool(0)))

## -----------------------------------------------------------------------------

def test_boolean_binary_operators():
    print True is False
    print True is not False

    # NOTE: Python prints arithmetic operations on bools as `1`s and `0`s, but we
    # actually print them as `True` and `False`.
    print (True + False) == 1
    print (True - False) == 1
    print (True * False) == 0
    print (False / True) == 0
    print (False % True) == 0
    print (True ** False) == 1
    print (False << True) == 1
    print (True >> False) == 1
    print (True | False) == 1
    print (bool(0) | False) == 1
    print (False & True) == 0
    print (True & bool(1)) == 1
    print (True ^ bool(0)) == 1
    print (bool(0) ^ bool(1)) == 1
    print (bool(1) // bool(1)) == 0

## -----------------------------------------------------------------------------

def test_boolean_aug_assignments():

    # NOTE: In Python (tested in v2.7.6), aug-assignments on instances of `bool`
    # type makes the instance itself turn into type `int`. This behavior, however,
    # it not currently supported.
    i = True
    i += False
    print i == (True + False)

    j = True
    j -= False
    print j == (True - False)

    k = False
    k *= bool(1)
    print k == (False * bool(1))

    u = bool(1)
    u /= bool(1)
    print u == (bool(1) / bool(1))

    v = bool(1)
    v %= True
    print v == (bool(1) % True)

    w = bool(1)
    w **= bool(0)
    print w == (bool(1) ** bool(0))

    x = True
    x <<= True
    print x == (True << True)

    y = False
    y >>= True
    print y == (False >> True)

    z = True
    z |= False
    print z == (True | False)

    zeta = bool(1)
    zeta ^= bool(0)
    print zeta == (bool(1) ^ bool(0))

    zelda = bool(0)
    zelda &= bool(1)
    print zelda == (bool(0) & bool(1))

    fd = True
    fd //= bool(1)
    print fd == (True // bool(1))

## -----------------------------------------------------------------------------

def test_boolean_comparisons():
    if True == bool(1):
        print 'Integrity of truth is rock solid'

    if False != True:
        print 'Opposite polarity do not match'

    if False < True:
        print 'The truth always triumphs'

    if bool(0) <= bool(1):
        print 'The truth still triumphs'

    if True > False:
        print 'The truth remains triumphant'

    if bool(100) >= bool(0):
        print 'The truth remains triumphant eternally'

## -----------------------------------------------------------------------------

def test_boolean_hash():
    print True.__hash__() == 1
    print False.__hash__() == 0

## -----------------------------------------------------------------------------

def test_boolean_nonzero():
    print True.__nonzero__()
    print False.__nonzero__()

    i = bool(-1)
    print i.__nonzero__()

    i = bool(0)
    print i.__nonzero__()

    i = bool(1)
    print i.__nonzero__()

## -----------------------------------------------------------------------------

test_boolean_representations()
test_boolean_unary_operators()
test_boolean_binary_operators()
test_boolean_aug_assignments()
test_boolean_comparisons()
test_boolean_hash()
test_boolean_nonzero()

## -----------------------------------------------------------------------------
