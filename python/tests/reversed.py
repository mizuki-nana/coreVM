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

def __test_reversed_with_object(obj):
    it = reversed(obj)

    try:
        while True:
            print next(it)
    except StopIteration:
        print 'STOP'

## -----------------------------------------------------------------------------

def test_reversed_on_types_without_reversed_defined():
    __test_reversed_with_object('')
    __test_reversed_with_object('A')
    __test_reversed_with_object('Hi')
    __test_reversed_with_object('Hello world')

    __test_reversed_with_object(tuple([]))
    __test_reversed_with_object(tuple([1]))
    __test_reversed_with_object(tuple([1, 2, 3, 4, 5]))

## -----------------------------------------------------------------------------

def test_reversed_on_types_with_reversed_define():
    __test_reversed_with_object(list([]))
    __test_reversed_with_object(list([1, 2, 3]))

## -----------------------------------------------------------------------------

def test_reversed_on_unsupported_types():
    def test_reversed_with_typerror(seq):
        try:
            reversed(seq)
        except TypeError:
            print 'Expected TypeError for object'

    test_reversed_with_typerror(1)
    test_reversed_with_typerror(True)
    test_reversed_with_typerror(set({1, 2, 3}))

    # Detect `dict` type is not a sequence.
    #test_reversed_with_typerror(dict({1:1, 2: 2}))

## -----------------------------------------------------------------------------

def test_reversed_on_custom_type():
    class MyObject(object):

        def __init__(self, value):
            self.value = value

        def __reversed__(self):
            return reversed(self.value)

    __test_reversed_with_object(MyObject('Hello world'))
    __test_reversed_with_object(MyObject(tuple([1, 2, 3])))

## -----------------------------------------------------------------------------

test_reversed_on_types_without_reversed_defined()
test_reversed_on_types_with_reversed_define()
test_reversed_on_unsupported_types()
test_reversed_on_custom_type()

## -----------------------------------------------------------------------------
