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

def test_sorted_with_one_argument():
    print sorted([])
    print sorted([0])
    print sorted([1])
    print sorted([2, 1, 3])
    print sorted([2, 3, 1])
    print sorted([1, 2, 3, 4, 5])
    print sorted([5, 4, 3, 2, 1, 0, -1])
    print sorted([2, -1, 3, -2])

## -----------------------------------------------------------------------------

def test_sorted_with_cmp():
    def cmp_(x, y):
        return cmp(y, x)

    print sorted([], cmp_)
    print sorted([0], cmp_)
    print sorted([1], cmp_)
    print sorted([2, 1, 3], cmp_)
    print sorted([2, 3, 1], cmp_)
    print sorted([1, 2, 3, 4, 5], cmp_)
    print sorted([5, 4, 3, 2, 1, 0, -1], cmp_)
    print sorted([2, -1, 3, -2], cmp_)

## -----------------------------------------------------------------------------

def test_sorted_with_key():
    key = lambda x: -(x)

    print sorted([], key=key)
    print sorted([0], key=key)
    print sorted([1], key=key)
    print sorted([2, 1, 3], key=key)
    print sorted([2, 3, 1], key=key)
    print sorted([1, 2, 3, 4, 5], key=key)
    print sorted([5, 4, 3, 2, 1, 0, -1], key=key)
    print sorted([2, -1, 3, -2], key=key)

## -----------------------------------------------------------------------------

def test_sorted_with_reverse():
    print sorted([], reverse=True)
    print sorted([0], reverse=True)
    print sorted([1], reverse=True)
    print sorted([2, 1, 3], reverse=True)
    print sorted([2, 3, 1], reverse=True)
    print sorted([1, 2, 3, 4, 5], reverse=True)
    print sorted([5, 4, 3, 2, 1, 0, -1], reverse=True)
    print sorted([2, -1, 3, -2], reverse=True)

## -----------------------------------------------------------------------------

def test_sorted_with_invalid_types():
    def test_sorted_with_typerror(iterable):
        try:
            print sorted(iterable)
        except TypeError:
            print 'Expected TyepError from calling sorted'

    test_sorted_with_typerror(1)
    test_sorted_with_typerror(True)
    test_sorted_with_typerror(3.14)

## -----------------------------------------------------------------------------

def test_sorted_with_custom_type():
    class MyObject(object):

        def __init__(self, name, value):
            self.name = name
            self.value = value

        def __repr__(self):
            return self.name

    my_objects = [MyObject('Garfield', 5), MyObject('Oddie', -2), MyObject('Nermo', 3), MyObject('John', 0)]
    print sorted(my_objects, key=lambda obj: obj.value)

## -----------------------------------------------------------------------------

test_sorted_with_one_argument()
test_sorted_with_cmp()
test_sorted_with_key()
test_sorted_with_reverse()
test_sorted_with_invalid_types()
test_sorted_with_custom_type()

## -----------------------------------------------------------------------------
