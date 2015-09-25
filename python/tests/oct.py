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

def test_oct_on_int_type():
    print oct(0)
    print oct(1)
    print oct(5)
    print oct(7)
    print oct(8)
    print oct(16)
    print oct(32)
    print oct(1024)
    print oct(-7)
    print oct(-8)
    print oct(-16)
    print oct(-32)
    print oct(-1024)
    print oct(-1024 * 1024)
    print oct(True)
    print oct(False)

## -----------------------------------------------------------------------------

def test_oct_on_custom_type():
    class MyObject(object):

        def __init__(self, x):
            self.x = x

        def __oct__(self):
            return self.x.__str__()

    print oct(MyObject(0))
    print oct(MyObject(7))
    print oct(MyObject(8))
    print oct(MyObject(16))
    print oct(MyObject(32))
    print oct(MyObject(1024))
    print oct(MyObject(-16))
    print oct(MyObject(-256))

## -----------------------------------------------------------------------------

def test_oct_on_unsupported_types():
    def test_oct_with_typeerror(x):
        try:
            print oct(x)
        except TypeError:
            print 'Expected TypeError for calling oct() on argument'

    test_oct_with_typeerror(3.14)
    test_oct_with_typeerror('5')
    test_oct_with_typeerror('32')
    test_oct_with_typeerror('32.0')
    test_oct_with_typeerror(list([1 ,2, 3]))

    class MyObject(object):
        def __init__(self):
            pass

    test_oct_with_typeerror(MyObject())

## -----------------------------------------------------------------------------

test_oct_on_int_type()
test_oct_on_custom_type()
test_oct_on_unsupported_types()

## -----------------------------------------------------------------------------
