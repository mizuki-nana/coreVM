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

def test_slice_representation():
    print slice(1, None, None)
    print slice(1, 2, None)
    print slice(1, 2, 3)

## -----------------------------------------------------------------------------

def test_hash():
    s = slice(1, 2, 3)
    try:
        s.__hash__()
    except TypeError:
        print 'slice is unhashable type'

## -----------------------------------------------------------------------------

def test_indices():
    print slice(1, 10, 2).indices(1)
    print slice(1, 10, 2).indices(10)
    print slice(1, 10, 2).indices(0)
    print slice(1, 10, 2).indices(-1)
    print slice(10, 100, 20).indices(5)
    print slice(10, 8, 5).indices(5)

    try:
        slice(10, 8, 0).indices(5)
    except ValueError:
        print 'slice step cannot be zero'

## -----------------------------------------------------------------------------

test_slice_representation()
test_hash()
test_indices()

## -----------------------------------------------------------------------------
