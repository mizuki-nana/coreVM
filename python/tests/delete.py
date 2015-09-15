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

def test_del_on_attribtues():
    class MyObject():
        def __init__(self):
            pass

        def do_something_stupid(self):
            print 'Hit against the wall'

    o = MyObject()
    o.do_something_stupid()

    # TODO: [COREVM-327] Implement `delattr` built-in function in Python
    #del MyObject.do_something_stupid

## -----------------------------------------------------------------------------

def test_del_on_names():
    i = 5
    del i

    j = 6
    def inner():
      k = 7
      del k

    inner()

    del j

## -----------------------------------------------------------------------------

def test_del_on_subscripts():
    def test_del_on_list_subscript():
        ls = [1, 2, 3, 4, 5]
        print ls

        del ls[3]
        print ls

        try:
            del ls[4]
        except IndexError:
            print 'This index has already been deleted'

    test_del_on_list_subscript()

    def test_del_on_dict_subscript():
        d = {'2': 2, '1': 1}
        print d

        del d['2']
        print d

        del d['1']
        print d

        d['0'] = 0
        print d

        del d['0']
        print d

        try:
            k = '1'
            del d[k]
        except KeyError:
            print 'This key has already been deleted'

    test_del_on_dict_subscript()

## -----------------------------------------------------------------------------

def test_del_on_name_lists():
    i = 5
    j = 6
    k = 7
    del [i, j, k]

## -----------------------------------------------------------------------------

def test_del_on_name_tuples():
    i = 5
    j = 6
    k = 7
    del (i, j, k)

## -----------------------------------------------------------------------------

test_del_on_attribtues()
test_del_on_names()
test_del_on_subscripts()
test_del_on_name_lists()
test_del_on_name_tuples()

## -----------------------------------------------------------------------------
