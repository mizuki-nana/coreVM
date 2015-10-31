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

class MyObject(object):

    def __init__(self, arg):
        pass

    def run(self, arg):
        print arg

## -----------------------------------------------------------------------------

def test_object_creation_and_interaction():
    o = MyObject(1)
    o.run('Hello world')
    o.name = 'Will'
    print o.name

## -----------------------------------------------------------------------------

def test_stmts_in_class_definitions():
    class MyObject(object):

        try:
            print 'Hello'

            # Tests shadowing of class names do not affect scopes outside
            # of class definition.
            MyObject = 1
            print MyObject

            name = 'Hello'
        except Exception:
            pass

        print MyObject + 1

        def __init__(self):
            print self.name

        print MyObject + 2

    o = MyObject()

## -----------------------------------------------------------------------------

test_object_creation_and_interaction()
test_stmts_in_class_definitions()

## -----------------------------------------------------------------------------
