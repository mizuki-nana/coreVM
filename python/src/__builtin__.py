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

class function:
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

class comprehension(object):

    def __init__(self, callee, predicate=None):
        self.callee = callee
        self.predicate = predicate

    def eval(self, iterable):
        iterator = __call_method_0(iterable.__iter__)

        try:
            while True:
                item = __call_method_0(iterator.next)

                okay = True
                if self.predicate is not None:
                    okay = self.predicate(item)

                if okay:
                    __call_method_1(self.callee.eval, item)
        except StopIteration:
            pass

        return __call_method_0(self.result)

    def result(self):
        return __call_method_0(self.callee.result)

## -----------------------------------------------------------------------------

class generator(object):

    def __init__(self, elt, res, synthesizer):
        self.elt = elt
        self.res = res
        self.synthesizer = synthesizer

    def eval(self, item):
        item_ = self.elt(item)
        self.synthesizer(self.res, item_)

    def result(self):
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

def abs(x):
    """Built-in functions.

    Reference:
        https://docs.python.org/2/library/functions.html#abs
    """

    if hasattr(x, '__abs__'):
        return __call_method_0(x.__abs__)

    raise __call_cls_0(TypeError)

## -----------------------------------------------------------------------------

def all(iterable):
    """Built-in function.

    Reference:
        https://docs.python.org/2/library/functions.html#all
    """
    iterator = __call_method_0(iterable.__iter__)

    try:
        while True:
            element = __call_method_0(iterator.next)

            # TODO: convert `element` to an instance of `bool` is the most
            # accurate form of implementation here, since not objects of all
            # types support `__nonzero__`, e.g., `None`.
            #
            # [COREVM-328] Add support for universal type conversion to `bool` type
            if __call_method_0(__call_method_0(element.__nonzero__).__not__):
                return False

    except StopIteration:
        pass

    return True

## -----------------------------------------------------------------------------

def any(iterable):
    """Built-in function.

    Reference:
        https://docs.python.org/2/library/functions.html#any
    """
    iterator = __call_method_0(iterable.__iter__)

    try:
        while True:
            element = __call_method_0(iterator.next)

            # TODO: convert `element` to an instance of `bool` is the most
            # accurate form of implementation here, since not objects of all
            # types support `__nonzero__`, e.g., `None`.
            #
            # [COREVM-328] Add support for universal type conversion to `bool` type
            if __call_method_0(element.__nonzero__):
                return True

    except StopIteration:
        pass

    return False

## -----------------------------------------------------------------------------

def delattr(obj, name):
    """Built-in function.

    Reference:
        https://docs.python.org/2/library/functions.html#delattr
    """

    """
    ### BEGIN VECTOR ###
    [ldobj, obj, 0]
    [gethndl2, name, 0]
    [hasattr2, 0, 0]
    [lnot, 0, 0]
    [jmpif, 5, 0]
    ### END VECTOR ###
    """

    """
    ### BEGIN VECTOR ###
    [ldobj, obj, 0]
    [gethndl2, name, 0]
    [delattr2, 0, 0]
    [ldobj, None, 0]
    [rtrn, 0, 0]
    ### END VECTOR ###
    """

    raise __call_cls_0(AttributeError)

## -----------------------------------------------------------------------------

def getattr(obj, name):
    """Built-in function.

    Reference:
        https://docs.python.org/2/library/functions.html#getattr
    """

    """
    ### BEGIN VECTOR ###
    [ldobj, obj, 0]
    [gethndl2, name, 0]
    [hasattr2, 0, 0]
    [lnot, 0, 0]
    [jmpif, 3, 0]
    ### END VECTOR ###
    """

    """
    ### BEGIN VECTOR ###
    [gethndl2, name, 0]
    [getattr2, 0, 0]
    [rtrn, 0, 0]
    ### END VECTOR ###
    """

    raise __call_cls_0(AttributeError)

## -----------------------------------------------------------------------------

def hasattr(obj, name):
    """Built-in function.

    Reference:
        https://docs.python.org/2/library/functions.html#hasattr
    """

    try:
        getattr(obj, name)
        return True
    except AttributeError:
        return False

## -----------------------------------------------------------------------------

def setattr(obj, name, value):
    """Built-in function.

    Reference:
        https://docs.python.org/2/library/functions.html#setattr
    """

    # If target object is a class and attribute value is a function.
    # Turn the function into an instance of `MethodType`.
    if obj.__class__ is type and value.__class__ is function:
        value.__class__ = MethodType

    """
    ### BEGIN VECTOR ###
    [ldobj, obj, 0]
    [ldobj, value, 0]
    [gethndl2, name, 0]
    [setattr2, 0, 0]
    ### END VECTOR ###
    """

    return None

## -----------------------------------------------------------------------------
