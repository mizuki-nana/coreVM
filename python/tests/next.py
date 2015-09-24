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

def test_next_without_default_value():
    ls = [1, 2, 3, 4, 5]
    itr = iter(ls)

    try:
        while True:
            print next(itr)
    except StopIteration:
        print 'STOP'

## -----------------------------------------------------------------------------

def test_next_with_default_value():
    ls = [1, 2, 3, 4, 5, 6]

    itr = iter(ls)

    try:
        i = 0
        while True and i < 10:
            print next(itr, 7)
            i += 1
    except StopIteration:
        print 'This should not be hit'

## -----------------------------------------------------------------------------

def test_next_with_invalid_input():
    def test_next_with_typeerror(it):
        try:
            print next(it)
        except TypeError:
            print 'Expected TypeError for iterator'

    test_next_with_typeerror(1)
    test_next_with_typeerror(3.14)
    test_next_with_typeerror('hello world')
    test_next_with_typeerror(list([1, 2, 3]))
    test_next_with_typeerror(dict({1: 1, 2: 2}))

## -----------------------------------------------------------------------------

test_next_without_default_value()
test_next_with_default_value()
test_next_with_invalid_input()

## -----------------------------------------------------------------------------
