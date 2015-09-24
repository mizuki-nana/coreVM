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

def test_id_on_objects_of_same_type():
    print id(1) != id(2)
    print id(3.14) != id(3.14000001)
    print id('Hello') != id('world')

    print id(list([1, 2, 3])) != id(list([1, 2, 3]))
    print id(set({'a', 'b', 'c'})) != id(set({'a', 'b', 'c'}))
    print id(dict({1: 1, 2: 2})) != id(dict({1: 1, 2: 2}))

## -----------------------------------------------------------------------------

def test_id_on_objects_of_different_types():
    print id(1) != id('1')
    print id(0) != id(0.0)
    print id(list([1, 2, 3])) != id(tuple([1, 2, 3]))

## -----------------------------------------------------------------------------

def test_id_on_type_objects():
    print id(object) == id(object)
    print id(int) == id(int)
    print id(float) == id(float)
    print id(str) == id(str)
    print id(dict) == id(dict)

    print id(int) != id(object)
    print id(int) != id(float)
    print id(list) != id(tuple)
    print id(list) != id(set)
    print id(set) != id(dict)

## -----------------------------------------------------------------------------

def test_id_on_custom_type():
    class MyObject(object):
        def __init__(self):
            pass

    print id(MyObject) != id(object)
    print id(MyObject) == id(MyObject)

    o = MyObject()
    print id(MyObject()) != id(o)

## -----------------------------------------------------------------------------

test_id_on_objects_of_same_type()
test_id_on_objects_of_different_types()
test_id_on_type_objects()
test_id_on_custom_type()

## -----------------------------------------------------------------------------
