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

def test_list_repr():
    print list([])
    print list([1])
    print list([1, 2])
    print list([1, 2, list([3,4,5])])
    print ['Hello', 'World!', ['Welcome', 'to', 'Python', [123, True, 9.654321]]]

## -----------------------------------------------------------------------------

def test_hash_list():
    try:
        [1, 2, 3].__hash__()
    except TypeError:
        print 'TypeError: unhashable type: list'

## -----------------------------------------------------------------------------

def test_list_getitem():
    l = [1, 2, 3]
    print l[0]
    print l[1]
    print l[2]
    try:
        print l[4]
    except IndexError:
        print 'Index out of bound'

## -----------------------------------------------------------------------------

def test_list_setitem():
    l = [1, 2]
    print l[0]
    l[0] = 2
    print l[0]
    l[0] += 3
    print l[0]
    l[1] = 5
    print l
    l[1] *= 10
    print l
    try:
        l[1] = 10
    except IndexError:
        print 'Index out of bound'

    try:
        l[1] += 100
    except IndexError:
        print 'Index out of bound'

## -----------------------------------------------------------------------------

def test_list_comprehension():
    l = [1, 2, 3, 4, 5]
    print [i for i in l]

    c = 5
    print [i * c for i in l]

    print [i for i in l if i <= 3]

## -----------------------------------------------------------------------------

test_list_repr()
test_hash_list()
test_list_getitem()
test_list_setitem()
test_list_comprehension()

## -----------------------------------------------------------------------------
