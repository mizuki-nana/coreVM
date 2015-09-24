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

def __test_iter_with_object(obj):
    iterator = iter(obj)

    try:
        while True:
            item = iterator.next()
            print item
    except StopIteration:
        print 'STOP'

## -----------------------------------------------------------------------------

def __test_iter_with_object_and_sentinel(obj, sentinel):
    iterator = iter(obj, sentinel)

    try:
        while True:
            item = iterator.next()
            print item
    except StopIteration:
        print 'STOP'

## -----------------------------------------------------------------------------

def test_iter_with_objects_supporting_iteration_protocol():
    __test_iter_with_object(list([]))
    __test_iter_with_object(list([1, 2, 3]))
    __test_iter_with_object(set({'b', 'c', 'a'}))
    __test_iter_with_object('')
    __test_iter_with_object('Hello world')

    class MyObject(object):

        def __init__(self, iterable):
            self.iterable = iterable

        def __iter__(self):
            return self.iterable.__iter__()

    __test_iter_with_object(MyObject(list([1, 2, 3, 4, 5])))

## -----------------------------------------------------------------------------

def test_iter_with_objects_supporting_sequence_protocol():
    class MyObject(object):

        def __init__(self, iterable):
            self.iterable = iterable

        def __len__(self):
            return self.iterable.__len__()

        def __getitem__(self, i):
            return self.iterable.__getitem__(i)

    __test_iter_with_object(MyObject(list([])))
    __test_iter_with_object(MyObject(list([1, 10, 200, 3000, 40000, 500000])))

## -----------------------------------------------------------------------------

def test_iter_with_object_of_invalid_type():
    class MyObject(object):
        def __init__(self):
            pass

    try:
        iterator = iter(MyObject())
    except TypeError:
        print 'Expected TypeError for object'

## -----------------------------------------------------------------------------

def test_iter_with_specifying_sentinel_value():
    class MyObject(object):

        def __init__(self, iterable):
            self.iterable = iterable
            self.i = 0

        def __call__(self):
            val = self.iterable[self.i]
            self.i += 1
            return val

    __test_iter_with_object_and_sentinel(MyObject(list([1, 2, 3, 4, 5])), 1)
    __test_iter_with_object_and_sentinel(MyObject(list([1, 2, 3, 4, 5])), 3)
    __test_iter_with_object_and_sentinel(MyObject(list([1, 2, 3, 4, 5])), 5)

## -----------------------------------------------------------------------------

test_iter_with_objects_supporting_iteration_protocol()
test_iter_with_objects_supporting_sequence_protocol()
test_iter_with_object_of_invalid_type()
test_iter_with_specifying_sentinel_value()

## -----------------------------------------------------------------------------
