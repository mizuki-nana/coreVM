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

def test_filter_on_list_type_iterable():
    print filter(lambda x: x, [])
    print filter(lambda x: x > 3, [0, 1, 2, 3, 4, 5])
    print filter(lambda x: bool(x), [0, 1, 2, 3, 4, 5])
    print filter(lambda x: bool(x), [True, False])

## -----------------------------------------------------------------------------

def test_filter_when_function_is_none():
    print filter(None, [])
    print filter(None, [0, 1, 2, 3, 4, 5])
    print filter(None, [True, False])
    print filter(None, ['Hello', 'world', 'greeting!', ''])
    print filter(None, [False, None, 0])

## -----------------------------------------------------------------------------

def test_filer_on_str_type_iterable():
    print filter(None, '123')
    print filter(lambda x : x != ' ', 'Hello world')

## -----------------------------------------------------------------------------

def test_filter_on_tuple_type_iterable():
    print filter(None, tuple([0, 1, 2, 3, 4, 5]))
    print filter(lambda x: x > 3, tuple([0, 1, 2, 3, 4, 5]))

## -----------------------------------------------------------------------------

test_filter_on_list_type_iterable()
test_filter_when_function_is_none()
test_filer_on_str_type_iterable()
test_filter_on_tuple_type_iterable()

## -----------------------------------------------------------------------------
