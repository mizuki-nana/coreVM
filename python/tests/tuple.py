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

def test_tuple_representation():
    print (1, 2, 3, 4, 5)
    print (1, 'Hello world!', ['Welcome', 'to', 'Python'], {1: 101}, (99, True))

    print (1, 2, 3, 4) + ('Hello world', [True, False], (5, 6) + (7, 8)) + ({1: 11}, {2: 22})

## -----------------------------------------------------------------------------

def test_iterate_tuples():
    for i in (1, 2, 3, 4, 5):
        print i

## -----------------------------------------------------------------------------

def test_tuple_hash():
    print (1, 2, 3).__hash__() > 0
    print (1, 2, 3).__hash__() != (4, 5, 6).__hash__()

## -----------------------------------------------------------------------------

def test_tuple_slicing():
    t = (1, 2, 3, 4, 5, 6, 7, 8, 9, 10)
    t2 = ()

    print t

    print t[1]
    print t[1:1]
    print t[1:5]
    print t[:9]
    print t[5:]
    print t[0:0]
    print t[8:2]
    print t[9:9]
    print t[1:8:2]
    print t[::8]
    #print t[::10] # Python prints trailing coma.
    print t[1:8:-1]
    print t[1:8:-5]
    print t[::]
    print t[::-1]
    print t[::-8]
    print t[1:1:-1]
    print t[9:9:-1]
    print t[-1:-1:-1]

    print t2[1:1]
    print t2[::1]
    print t2[::-1]
    print t2[1:8:-2]
    print t2[-1:-1:-1]

    try:
        t[::0]
    except ValueError:
        print 'Slice step cannot be zero'

    print t
    print t2

## -----------------------------------------------------------------------------

test_tuple_representation()
test_iterate_tuples()
test_tuple_hash()
test_tuple_slicing()

## -----------------------------------------------------------------------------
