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

def test_hasattr_on_builtin_types():
    assert hasattr(1, '__pos__')
    assert hasattr(3.1415, '__mul__')
    assert hasattr('Hello world', '__str__')
    assert hasattr(set({1, 2, 3}), 'symmetric_difference')

    assert hasattr(3, '__compute__') == False
    assert hasattr(0.001, 'sqrt') == False
    assert hasattr([1, 2, 3], 'erase') == False

    assert hasattr(object, '__new__')
    assert hasattr(int, '__neg__')
    assert hasattr(dict, '__getitem__')

    assert hasattr(str, '__tostring__') == False
    assert hasattr(complex, '__simplify__') == False
    assert hasattr(bool, '__truthy__') == False

## -----------------------------------------------------------------------------

def test_hasattr_on_custom_type():
    class MyObject(object):

        def __init__(self):
            self.value = 5

        def hello_world(self):
            print 'Hello'

    assert hasattr(MyObject, '__init__')
    assert hasattr(MyObject, 'hello_world')
    assert hasattr(MyObject, 'value') == False

    o = MyObject()

    assert hasattr(o, '__init__')
    assert hasattr(o, 'hello_world')
    assert hasattr(o, 'value')

## -----------------------------------------------------------------------------

test_hasattr_on_builtin_types()
test_hasattr_on_custom_type()

## -----------------------------------------------------------------------------
