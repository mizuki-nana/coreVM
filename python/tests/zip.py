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

def test_zip_with_one_iterable():
    print zip([])
    print zip([1])
    print zip([1, 2, 3])

## -----------------------------------------------------------------------------

def test_zip_with_multiple_iterables():
    print zip([1, 2, 3], [4, 5, 6], [7, 8, 9])
    print zip(['a', 'e', 'x'], ['b', 'f', 'y'], ['c', 'g', 'z'])

## -----------------------------------------------------------------------------

def test_zip_with_multiple_iterables_of_different_lengths():
    print zip([], [1])
    print zip([1, 2, 3], [4, 5, 6, 7, 8, 9])

## -----------------------------------------------------------------------------

def test_zip_with_zero_iterables():
    print zip()

## -----------------------------------------------------------------------------

def test_zip_with_invalid_arguments():
    def test_zip_with_typeerror(*args):
        try:
            print zip(*args)
        except TypeError:
            print 'Expected TypeError'

    test_zip_with_typeerror(1)
    test_zip_with_typeerror([1, 2], [3, 4], 5)
    test_zip_with_typeerror('Hello', 'world')

## -----------------------------------------------------------------------------

test_zip_with_one_iterable()
test_zip_with_multiple_iterables()
test_zip_with_multiple_iterables_of_different_lengths()
test_zip_with_zero_iterables()
test_zip_with_invalid_arguments()

## -----------------------------------------------------------------------------
