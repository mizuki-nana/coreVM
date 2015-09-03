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

def test_ifelse_expr():
    print 'Nested if-esle WTF :D' if (True if 1 + 1 == 2 else False) \
        else 'This is too confusing @__@'

## -----------------------------------------------------------------------------

def false():
    print 'false'
    return False

## -----------------------------------------------------------------------------

def test_boolop_expr():
    if 1 or 0:
        print '1 or 0 == True'

    if 0 or 0:
        print 'This cannot be said'

    if 1 and 0:
        print 'This is a secret'

    if 0 and 0:
        print 'This cannot be said too'

    if 1 and 2 and 3 or 4:
        print 'Hello'

    if 1 and 2 and 0:
        print 'Be quite!!!'

    if (1 and 2) or (3 and 0):
        print 'This is now public'

    if (0 and 1) and (3 and 4):
        print 'Skip this round'

    if True is False and False or not False:
        print 'This is confusing...'

    if True and false():
        print 'This is False (THIS SHOULD NOT BE PRINTED)'

    if false() and True and True:
        print 'This is False (THIS SHOULD NOT BE PRINTED)'

    if (True or True) and false():
        print 'This is False (THIS SHOULD NOT BE PRINTED)'

    if (True or False) and false():
        print 'This is still False (THIS SHOULD NOT BE PRINTED)'

    if (True or True) and False and false():
        print 'This is still False (THIS SHOULD NOT BE PRINTED)'

    if (True or False) and False and false():
        print 'This is still False (THIS SHOULD NOT BE PRINTED)'

    if (False or 0) and (1 and 0):
        print 'This is False (THIS SHOULD NOT BE PRINTED)'

    if True and True and 0:
        print 'This is False (THIS SHOULD NOT BE PRINTED)'

    print True and false()
    print True and false() and True

    if 0 or 0.0 or False:
        print 'Too much negativity is bad (THIS SHOULD NOT BE PRINTED)'
    elif 0 or 0.0 or False or True:
        print 'Gotta have something positive'

## -----------------------------------------------------------------------------

def test_comparison_exprs():
    print 1 < 2
    print True is True
    print True is False
    print False is not True
    print False is not 1
    print False is True is False

    print True is not False is not True
    print True is not False is not False
    print False is not False is not True
    print False is not False is not False

    print False is True is not False is not True
    print False is True is not False is not True is False
    print False is not True is True is not True is not True is not False
    print True is not True is True is not True is not True is not False is True

    print 1 <= 3 > 2
    print 2 <= 5 > 6
    print 3 > 4 < 5
    print 1 > 2 > 3

## -----------------------------------------------------------------------------

test_ifelse_expr()
test_boolop_expr()
test_comparison_exprs()

## -----------------------------------------------------------------------------
