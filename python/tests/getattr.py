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

def test_getattr(obj, name):
    try:
        getattr(obj, name)
        print 'It worked'
    except AttributeError:
        print 'ERROR'

## -----------------------------------------------------------------------------

def test_getattr_on_builtin_types():
    test_getattr(1, '__pos__')
    test_getattr(3.1415, '__mul__')
    test_getattr('Hello world', '__str__')
    test_getattr(set({1, 2, 3}), 'symmetric_difference')

    test_getattr(3, '__compute__')
    test_getattr(0.001, 'sqrt')
    test_getattr([1, 2, 3], 'erase')

    test_getattr(object, '__new__')
    test_getattr(int, '__neg__')
    test_getattr(dict, '__getitem__')

    test_getattr(str, '__tostring__')
    test_getattr(complex, '__simplify__')
    test_getattr(bool, '__truthy__')

## -----------------------------------------------------------------------------

def test_getattr_on_custom_type():
    class MyObject(object):

        def __init__(self):
            self.value = 5

        def hello_world(self):
            print 'Hello'

    test_getattr(MyObject, '__init__')
    test_getattr(MyObject, 'hello_world')
    test_getattr(MyObject, 'value')

    o = MyObject()

    test_getattr(o, '__init__')
    test_getattr(o, 'hello_world')
    test_getattr(o, 'value')

## -----------------------------------------------------------------------------

test_getattr_on_builtin_types()
test_getattr_on_custom_type()

## -----------------------------------------------------------------------------
