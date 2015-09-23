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

def test_hash_on_int_type():
    print hash(0)
    print hash(1) > 0
    print hash(0) == hash(0)
    print hash(0) != hash(1)

## -----------------------------------------------------------------------------

def test_hash_on_float_type():
    print hash(0.0)
    print hash(0.1) > 0
    print hash(3.14) != hash(3)
    print hash(3.14) != hash(3.0)

## -----------------------------------------------------------------------------

def test_hash_on_str_type():
    print hash('') >= 0
    print hash('1') != hash(1)
    print hash('Hello') != hash('hello')
    print hash('Hello') != hash('World')

## -----------------------------------------------------------------------------

def test_hash_on_custom_type():
    class MyObject(object):

        def __init__(self):
            pass

    o = MyObject()
    print hash(o) > 0
    print hash(o) == hash(o)

    o2 = MyObject()
    print hash(o) != hash(o2)

    class YourObject(object):

        def __init__(self):
            pass

        def __hash__(self):
            return 5

    o = YourObject()
    print hash(o)

    print hash(YourObject()) == hash(o)

    print hash(YourObject()) != hash(MyObject())

## -----------------------------------------------------------------------------

test_hash_on_int_type()
test_hash_on_float_type()
test_hash_on_str_type()
test_hash_on_custom_type()

## -----------------------------------------------------------------------------
