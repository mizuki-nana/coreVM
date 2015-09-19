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

def test_all(iterable, expected):
    assert all(iterable) == expected

## -----------------------------------------------------------------------------

def test_all_with_empty_list():
    test_all([], True)

## -----------------------------------------------------------------------------

def test_all_with_single_element():
    test_all([-1], True)
    test_all([0], False)
    test_all([1], True)

    test_all([True], True)
    test_all([False], False)

## -----------------------------------------------------------------------------

def test_all_with_multiple_elements():
    test_all([1, 2, 3], True)
    test_all([True, False], False)
    test_all([1, 2, 0], False)

## -----------------------------------------------------------------------------

test_all_with_empty_list()
test_all_with_single_element()
test_all_with_multiple_elements()

## -----------------------------------------------------------------------------
