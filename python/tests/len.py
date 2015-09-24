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

def test_len_on_supporting_types():
    print len('Hello world')
    print len(list([1, 2, 3, 4, 5]))
    print len(tuple([1, 2, 3, 4, 5]))
    print len(set({1, 2, 3, 4, 5}))
    print len(dict({1: 1, 2: 2, 3: 3}))

    class MyObject(object):

        def __init__(self, val):
            self.val = val

        def __len__(self):
            return self.val.__len__()

    print len(MyObject(list([1, 2, 3])))

## -----------------------------------------------------------------------------

def test_len_on_unsupported_types():
    def test_len_with_typeerror(s):
        try:
            len(s)
        except TypeError:
            print 'Expected TypeError on calling len() on object'

    test_len_with_typeerror(1)
    test_len_with_typeerror(1.0)
    test_len_with_typeerror(True)

    class MyObject(object):
        def __init__(self):
            pass

    test_len_with_typeerror(MyObject())

## -----------------------------------------------------------------------------

test_len_on_supporting_types()
test_len_on_unsupported_types()

## -----------------------------------------------------------------------------
