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

def test_delattr_on_custom_type():
    class MyObject(object):

        def __init__(self):
            self.value = 5

        def run(self):
            return 'I am running...'

    o = MyObject()

    print o.value
    print o.run()

    res = delattr(o, 'value')
    print res

    try:
        print getattr(o, 'value')
    except AttributeError:
        print 'Attribute deleted'

    try:
        delattr(o, 'some random attribute')
    except AttributeError:
        print 'Attribute does not exist'

    res = delattr(MyObject, 'run')
    print res

    o2 = MyObject()

    try:
        print getattr(o2, 'run')
    except AttributeError:
        print 'Attribute deleted'

## -----------------------------------------------------------------------------

test_delattr_on_custom_type()

## -----------------------------------------------------------------------------
