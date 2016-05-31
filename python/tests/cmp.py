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

def test_cmp_on_builtin_types():
    print cmp(1, 2)
    print cmp(1, 0)
    print cmp(1, -1)

    print cmp(3.14, 3.15)
    print cmp(6.2897, 3.14)
    print cmp(123.456789, 123.456789)

    print cmp(True, True)
    print cmp(True, False)
    print cmp(False, True)
    print cmp(False, False)

    print cmp('Hello', 'world')
    print cmp('world', 'Hello')
    print cmp('Hello', 'Hello')

    print cmp(list([1, 2, 3]), list([4, 5, 6]))
    print cmp(list([1, 2]), list([3]))
    print cmp(list([3]), list([1, 2]))
    print cmp(list([1, 2, 3]), list([1, 2, 3, 4]))

    print cmp(dict({1:1}), dict({1:1}))
    print cmp(dict({1:1}), dict({1:2}))
    print cmp(dict({1:2}), dict({1:1}))

## -----------------------------------------------------------------------------

def test_cmp_on_unsupported_types():
    def test_cmp_with_typeerror(x, y):
        try:
            print cmp(x, y)
        except TypeError:
            print 'Expected TypeError when calling cmp on ' + x.__str__()

    test_cmp_with_typeerror(complex(1, 2), complex(3, 4))
    test_cmp_with_typeerror(set({2, 1}), set({3, 4}))

    class MyObject(object):

        def __init__(self):
            pass

    test_cmp_with_typeerror(MyObject(), MyObject())

## -----------------------------------------------------------------------------

def test_cmp_on_custom_type():
    class MyObject(object):

        def __init__(self, x):
            self.x = x

        def __cmp__(self, other):
            return cmp(self.x, other.x)

    print cmp(MyObject(1), MyObject(2))
    print cmp(MyObject('B'), MyObject('A'))
    print cmp(MyObject(3.141592), MyObject(3.141592))

## -----------------------------------------------------------------------------

test_cmp_on_builtin_types()
test_cmp_on_unsupported_types()
test_cmp_on_custom_type()

## -----------------------------------------------------------------------------
