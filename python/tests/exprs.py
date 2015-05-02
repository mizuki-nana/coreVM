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

    if 0 or 0.0 or False:
        print 'Too much negativity is bad'
    elif 0 or 0.0 or False or True:
        print 'Gotta have something positive'

## -----------------------------------------------------------------------------

test_ifelse_expr()
test_boolop_expr()

## -----------------------------------------------------------------------------
