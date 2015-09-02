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

def greetings(func):
    def inner():
        print 'Hi'
        func()

    return inner

## -----------------------------------------------------------------------------

def polite(before, after):
    print before, after

    def inner(func):
        # TODO: [COREVM-309] Add support for Python function closures.
        #print before

        return func

        # TODO: [COREVM-309] Add support for Python function closures.
        #print after

    return inner

## -----------------------------------------------------------------------------

def test_simple_decoator():
    @greetings
    def inner():
        print 'How are you'

    inner()

## -----------------------------------------------------------------------------

def test_complex_decorator():
    @polite('Hi', 'Bye')
    def inner():
        print 'How are you'

    inner()

## -----------------------------------------------------------------------------

def test_chained_decorators():
    @polite('Hi', 'Bye')
    @greetings
    def inner():
        print 'How are you'

    inner()

## -----------------------------------------------------------------------------

def test_chained_decorators_2():
    @greetings
    @polite('Hi', 'Bye')
    def inner():
        print 'How are you again'

    inner()

## -----------------------------------------------------------------------------

def test_chained_decorators_3():
    @polite('Hi', 'Bye')
    @polite('Hello', 'See ya')
    def inner():
        print 'How are you this time'

    inner()

## -----------------------------------------------------------------------------

def add_id_dec(cls):
    orig_init = cls.__init__

    def __init__(self, *args, **kwargs):
        # TODO: [COREVM-309] Add support for Python function closures.
        #orig_init(self, *args, **kwargs)

        self.id = 1

    cls.__init__ = __init__

    return cls

## -----------------------------------------------------------------------------

def test_simple_cls_decorator():
    @add_id_dec
    class DummyClass(object):

        def __init__(self):
            print 'DummyClass.__init__'

    obj = DummyClass()
    print obj.id

## -----------------------------------------------------------------------------

def add_name_dec(name):
    print name

    def inner(cls):

      # TODO: [COREVM-309] Add support for Python function closures.
      # cls.name = name

      cls.name = 'Some class'

      return cls

    return inner

## -----------------------------------------------------------------------------

def test_complex_cls_decorator():
    @add_name_dec('Hello')
    class DummyClass(object):
        def __init__(self):
            print 'DummyClass.__init__'

    obj = DummyClass()
    print obj.name

## -----------------------------------------------------------------------------

def test_chained_cls_decorator_1():
    @add_id_dec
    @add_id_dec
    class DummyClass(object):
        def __init__(self):
            print 'DummyClass.__init__'

    obj = DummyClass()
    print obj.id

## -----------------------------------------------------------------------------

def test_chained_cls_decorator_2():
    @add_name_dec('Hello')
    @add_id_dec
    class DummyClass(object):
        def __init__(self):
            print 'DummyClass.__init__'

    obj = DummyClass()
    print obj.name, obj.id

## -----------------------------------------------------------------------------

def test_chained_cls_decorator_3():
    @add_id_dec
    @add_name_dec('Hello')
    class DummyClass(object):
        def __init__(self):
            print 'DummyClass.__init__'

    obj = DummyClass()
    print obj.name, obj.id

## -----------------------------------------------------------------------------

def test_chained_cls_decorator_4():
    @add_name_dec('Hello')
    @add_name_dec('World')
    class DummyClass(object):
        def __init__(self):
            print 'DummyClass.__init__'

    obj = DummyClass()
    print obj.name

## -----------------------------------------------------------------------------

# TODO: [COREVM-309] Add support for Python function closures.
#test_simple_decoator()

test_complex_decorator()

# TODO: [COREVM-309] Add support for Python function closures.
#test_chained_decorators()
#test_chained_decorators_2()

test_chained_decorators_3()

# Class decorators.
test_simple_cls_decorator()
test_complex_cls_decorator()
test_chained_cls_decorator_1()
test_chained_cls_decorator_2()
test_chained_cls_decorator_3()
test_chained_cls_decorator_4()

## -----------------------------------------------------------------------------
