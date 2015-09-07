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

def test_str_representation():
    print str("Hello world")
    print str(str("Hi again!!!"))
    print 'Today is Earth Day!'

## -----------------------------------------------------------------------------

def test_str_hash():
    print 'Hello world'.__hash__() > 0
    print 'Me'.__hash__() != 'You'.__hash__()

## -----------------------------------------------------------------------------

def test_str_equality():
    print 'Hello' == 'Hello'
    print 'Hello' != 'Hello'
    print 'Hello' != 'World'
    print 'Hello' == 'World'

## -----------------------------------------------------------------------------

def test_str_slicing():
    s = 'Hello world'
    s2 = ''

    print s

    print s[1]
    print s[1:1]
    print s[1:5]
    print s[:10]
    print s[5:]
    print s[0:0]
    print s[8:2]
    print s[10:10]
    print s[1:8:2]
    print s[::8]
    print s[::11]
    print s[1:8:-1]
    print s[1:8:-5]
    print s[::]
    print s[::-1]
    print s[::-8]
    print s[1:1:-1]
    print s[10:10:-1]
    print s[-1:-1:-1]

    print s2[::1]
    print s2[::1]
    print s2[::-1]
    print s2[1:8:-2]
    print s2[-1:-1:-1]

    try:
        s[::0]
    except ValueError:
        print 'Slice step cannot be zero'

    print s
    print s2

## -----------------------------------------------------------------------------

test_str_representation()
test_str_hash()
test_str_equality()
test_str_slicing()

## -----------------------------------------------------------------------------
