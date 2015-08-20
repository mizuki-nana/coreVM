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

def test_if():
    def test_if_inner(x):
        if x:
            print 'True'

    test_if_inner(1)
    test_if_inner(1 + 2)
    test_if_inner(True)

## -----------------------------------------------------------------------------

def test_if_else():
    def test_if_else_inner(x):
        if x:
            print 'True'
        else:
            print 'False'

    test_if_else_inner(1)
    test_if_else_inner(0)
    test_if_else_inner(True)
    test_if_else_inner(False)
    test_if_else_inner(0 ** 2)
    test_if_else_inner(0 / 2)
    test_if_else_inner(1 or 0)
    test_if_else_inner(True and False)

## -----------------------------------------------------------------------------

def test_if_elif_else():
    def test_if_elif_else_inner(x):
        if x % 2 == 0:
            print 'X is a multiple of 2'
        elif x % 3 == 0:
            print 'X is a multiple of 3'
        elif x % 5 == 0:
            print 'X is a multiple of 5'
        else:
            print 'X is a prime number'
            if x == 1:
                print 'X is one'
            else:
                print x

    test_if_elif_else_inner(2)
    test_if_elif_else_inner(3)
    test_if_elif_else_inner(5)
    test_if_elif_else_inner(1)
    test_if_elif_else_inner(7)

## -----------------------------------------------------------------------------

test_if()
test_if_else()
test_if_elif_else()

## -----------------------------------------------------------------------------
