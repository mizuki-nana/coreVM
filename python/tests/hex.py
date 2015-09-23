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

def test_hex_on_int_type():
    print hex(0)
    print hex(1)
    print hex(5)
    print hex(15)
    print hex(16)
    print hex(32)
    print hex(1024)
    print hex(-6)
    print hex(-16)
    print hex(-32)
    print hex(-1024)
    print hex(-1024 * 1024)
    print hex(True)
    print hex(False)

## -----------------------------------------------------------------------------

def test_hex_on_custom_type():
    class MyObject(object):

        def __init__(self, x):
            self.x = x

        def __hex__(self):
            return self.x.__str__()

    print hex(MyObject(0))
    print hex(MyObject(5))
    print hex(MyObject(16))
    print hex(MyObject(32))
    print hex(MyObject(1024))
    print hex(MyObject(-16))
    print hex(MyObject(-256))

## -----------------------------------------------------------------------------

def test_hex_on_unsupported_types():
    def test_hex_with_typeerror(x):
        try:
            print hex(x)
        except TypeError:
            print 'Expected TypeError for calling hex() on ' + x.__str__()

    test_hex_with_typeerror('5')
    test_hex_with_typeerror('32')
    test_hex_with_typeerror('32.0')
    test_hex_with_typeerror(list([1 ,2, 3]))

## -----------------------------------------------------------------------------

test_hex_on_int_type()
test_hex_on_custom_type()
test_hex_on_unsupported_types()

## -----------------------------------------------------------------------------
