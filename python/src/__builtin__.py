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

class type:
    pass

## -----------------------------------------------------------------------------

class MethodType:
    pass

## -----------------------------------------------------------------------------

class object:

    def __new__(cls):
        """
        ### BEGIN VECTOR ###
        [new, 0, 0]
        [ldobj, cls, 0]
        [setattrs2, im_self, 0]
        [ldobj, cls, 0]
        [setattr, __class__, 0]
        ### END VECTOR ###
        """

## -----------------------------------------------------------------------------

def __call(caller, *args, **kwargs):
    # Need to support *args and **kwargs.
    if caller.__class__ is type:
        obj = object.__new__(caller, *args, **kwargs)
        caller.__init__(obj, *args, **kwargs)
        return obj
    elif caller.__class__ is MethodType:
        return caller(caller.im_self, *args, **kwargs)
    else:
        return caller(*args, **kwargs)

## -----------------------------------------------------------------------------

def __call_cls(caller, *args, **kwargs):
    obj = object.__new__(caller, *args, **kwargs)
    caller.__init__(obj, *args, **kwargs)
    return obj

## -----------------------------------------------------------------------------

def __call_cls_0(caller):
    obj = object.__new__(caller)
    caller.__init__(obj)
    return obj

## ----------------------------------------------------------------------------

def __call_cls_1(caller, arg):
    obj = object.__new__(caller, arg)
    caller.__init__(obj, arg)
    return obj

## -----------------------------------------------------------------------------

def __call_cls_2(caller, arg1, arg2):
    obj = object.__new__(caller, arg1, arg2)
    caller.__init__(obj, arg1, arg2)
    return obj

## -----------------------------------------------------------------------------

def __call_cls_3(caller, arg1, arg2, arg3):
    obj = object.__new__(caller, arg1, arg2, arg3)
    caller.__init__(obj, arg1, arg2, arg3)
    return obj

## -----------------------------------------------------------------------------

def __call_cls_4(caller, arg1, arg2, arg3, arg4):
    obj = object.__new__(caller, arg1, arg2, arg3, arg4)
    caller.__init__(obj, arg1, arg2, arg3, arg4)
    return obj

## -----------------------------------------------------------------------------

def __call_cls_5(caller, arg1, arg2, arg3, arg4, arg5):
    obj = object.__new__(caller, arg1, arg2, arg3, arg4, arg5)
    caller.__init__(obj, arg1, arg2, arg3, arg4, arg5)
    return obj

## -----------------------------------------------------------------------------

def __call_cls_builtin(cls, obj):
    # Creates an object of builtin type by setting class properties directly
    # on the raw object (boxing it).
    """
    ### BEGIN VECTOR ###
    [ldobj, obj, 0]
    [ldobj, cls, 0]
    [setattrs2, im_self, 0]
    [ldobj, cls, 0]
    [setattr, __class__, 0]
    ### END VECTOR ###
    """
    return obj

## -----------------------------------------------------------------------------

def __call_method(caller, *arg):
    return caller(caller.im_self, *arg)

## -----------------------------------------------------------------------------

def __call_method_0(caller):
    return caller(caller.im_self)

## -----------------------------------------------------------------------------

def __call_method_1(caller, arg):
    return caller(caller.im_self, arg)

## -----------------------------------------------------------------------------

def __call_method_2(caller, arg1, arg2):
    return caller(caller.im_self, arg1, arg2)

## -----------------------------------------------------------------------------

def __call_method_3(caller, arg1, arg2, arg3):
    return caller(caller.im_self, arg1, arg2, arg3)

## -----------------------------------------------------------------------------

class sequence_generator(object):

    def __init__(self, iterable, elt, res, synthesizer, predicate):
        # TODO: make `predicate` a keyword argument once support for **kwargs
        # is added.
        self.iterable = iterable
        self.elt = elt
        self.res = res
        self.synthesizer = synthesizer
        self.predicate = predicate

    def eval(self):
        iterator = __call_method_0(self.iterable.__iter__)

        try:
            while True:
                next_item = __call_method_0(iterator.next)

                # TODO: associate types with lambdas, so that we can do
                # None check on them.
                okay = True
                if self.predicate is not None:
                    okay = self.predicate(next_item)

                if okay:
                    item = self.elt(next_item)

                    self.synthesizer(self.res, item)
        except StopIteration:
            pass

        return self.res

## -----------------------------------------------------------------------------

class dict_pair_generator(object):

    def __init__(self, iterable, elt, res, synthesizer, predicate):
        # TODO: make `predicate` a keyword argument once support for **kwargs
        # is added.
        self.iterable = iterable
        self.elt = elt
        self.res = res
        self.synthesizer = synthesizer
        self.predicate = predicate

    def eval(self):
        iterator = __call(self.iterable.__iter__)

        try:
            while True:
                key_value_pair = __call_method_0(iterator.next)

                key = key_value_pair.key
                value = key_value_pair.value

                # TODO: associate types with lambdas, so that we can do
                # None check on them.
                okay = True
                if self.predicate is not None:
                    okay = self.predicate(key, value)

                if okay:
                    item = self.elt(key, value)

                    self.synthesizer(self.res, item.key, item.value)
        except StopIteration:
            pass

        return self.res

## -----------------------------------------------------------------------------

class dict_item_generator(object):

    def __init__(self, iterable, elt, res, synthesizer, predicate):
        # TODO: make `predicate` a keyword argument once support for **kwargs
        # is added.
        self.iterable = iterable
        self.elt = elt
        self.res = res
        self.synthesizer = synthesizer
        self.predicate = predicate

    def eval(self):
        iterator = __call_method_0(self.iterable.__iter__)

        try:
            while True:
                next_item = __call_method_0(iterator.next)

                # TODO: associate types with lambdas, so that we can do
                # None check on them.
                okay = True
                if self.predicate is not None:
                    okay = self.predicate(next_item)

                if okay:
                    item = self.elt(next_item)

                    self.synthesizer(self.res, item.key, item.value)
        except StopIteration:
            pass

        return self.res

## -----------------------------------------------------------------------------

class with_stmt_runner(object):

    def __init__(self, manager, body):
        self.manager = manager
        self.body = body

    def run(self):
        """Executes a context managed block of code.

        Reference:
            https://docs.python.org/2.7/reference/datamodel.html#with-statement-context-managers
        """
        # Currently passing `exc_type` and `exc_tb` as `None`.
        # TODO: [COREVM-298] Capture Python exception type and stack trace for context managers
        exc_type = None
        exc_val = None
        exc_tb = None

        val = __call_method_0(self.manager.__enter__)

        try:
            __call(self.body, val)
        except Exception as ex:
            exc_val = ex

        suppress_exception = __call_method_3(
            self.manager.__exit__, exc_type, exc_val, exc_tb)

        if exc_val is not None:
            suppress = __call_cls_1(bool, suppress_exception)

            if __call_method_0(suppress.__not__):
                raise exc_val

## -----------------------------------------------------------------------------

def max(a, b):
    if __call_method_1(a.__gte__, b):
        return a

    return b

## -----------------------------------------------------------------------------

def min(a, b):
    if __call_method_1(a.__lte__, b):
        return a

    return b

## -----------------------------------------------------------------------------
