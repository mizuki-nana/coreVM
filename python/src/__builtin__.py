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

def bin(x):
    """Built-in function.

    Reference:
        https://docs.python.org/2/library/functions.html#bin
    """

    def __bin_on_int(x):
        CONST_INT_0 = __call_cls_builtin(int, 0)
        CONST_INT_2 = __call_cls_builtin(int, 2)
        CONST_STR_0 = __call_cls_builtin(str, '0')

        res = __call_cls_1(str, '')

        divident = abs(__call_cls_1(int, x))
        divisor = CONST_INT_2

        # Special case if `x` is 0.
        if __call_method_1(divident.__eq__, CONST_INT_0):
            __call_method_1(res.__add__, CONST_STR_0)

        # Loop until divident is no greater than 0.
        #
        # Use the "decimal-to-binary" algorithm taught in middle school
        # http://www.wikihow.com/Convert-from-Decimal-to-Binary
        while __call_method_1(divident.__gt__, CONST_INT_0):
            remainder = __call_method_1(divident.__mod__, divisor)

            __call_method_1(res.__add__, __call_method_0(remainder.__repr__))

            divident = __call_method_1(divident.__div__, divisor)

        # Add reversed binary prefix to end of result, and reverse
        # the entire result character sequence.
        __call_method_1(res.__add__, 'b0')

        # Negative sign if the original input is negative.
        if __call_method_1(x.__lt__, CONST_INT_0):
            __call_method_1(res.__add__, '-')

        """
        ### BEGIN VECTOR ###
        [ldobj, res, 0]
        [gethndl, 0, 0]
        [reverse, 0, 0]
        [sethndl, 0, 0]
        ### END VECTOR ###
        """
        return res

    if x.__class__ is int or x.__class__ is bool:
        return __bin_on_int(x)

    # Non-integer types.
    # If object has the `__index__` attribute, use its value
    # as the input, otherwise raise an instance of `TypeError`.
    if hasattr(x, '__index__'):
        return __bin_on_int(__call_method_0(x.__index__))

    raise __call_cls_0(TypeError)

## -----------------------------------------------------------------------------

def chr(i):
    """Built-in function.

    Reference:
        https://docs.python.org/2/library/functions.html#chr
    """
    LOWER_BOUND_I = __call_cls_builtin(int, 0)
    UPPER_BOUND_I = __call_cls_builtin(int, 255)

    # Valid input range is [0, 255].
    if __call_method_1(i.__lt__, LOWER_BOUND_I) or __call_method_1(i.__gt__, UPPER_BOUND_I):
        raise __call_cls_0(ValueError)

    # Special cases
    if __call_method_1(i.__eq__, __call_cls_builtin(int, 9)):
        return __call_cls_builtin(str, '\t')

    if __call_method_1(i.__eq__, __call_cls_builtin(int, 10)):
        return __call_cls_builtin(str, '\n')

    if __call_method_1(i.__eq__, __call_cls_builtin(int, 13)):
        return __call_cls_builtin(str, '\r')

    # 32 to 126, inclusive.
    def __chr_for_32_to_126(i):
        CONST_ASCII_ARRAY = __call_cls_builtin(list,
            [
                __call_cls_builtin(str, ' '), # 32
                __call_cls_builtin(str, '!'), # 33
                __call_cls_builtin(str, '"'), # 34
                __call_cls_builtin(str, '#'), # 35
                __call_cls_builtin(str, '$'), # 36
                __call_cls_builtin(str, '%'), # 36
                __call_cls_builtin(str, '&'), # 38
                __call_cls_builtin(str, '\''), # 39
                __call_cls_builtin(str, '('), # 40
                __call_cls_builtin(str, ')'), # 41
                __call_cls_builtin(str, '*'), # 42
                __call_cls_builtin(str, '+'), # 43
                __call_cls_builtin(str, ','), # 44
                __call_cls_builtin(str, '-'), # 45
                __call_cls_builtin(str, '.'), # 46
                __call_cls_builtin(str, '/'), # 47
                __call_cls_builtin(str, '0'), # 48
                __call_cls_builtin(str, '1'), # 49
                __call_cls_builtin(str, '2'), # 50
                __call_cls_builtin(str, '3'), # 51
                __call_cls_builtin(str, '4'), # 52
                __call_cls_builtin(str, '5'), # 53
                __call_cls_builtin(str, '6'), # 54
                __call_cls_builtin(str, '7'), # 55
                __call_cls_builtin(str, '8'), # 56
                __call_cls_builtin(str, '9'), # 57
                __call_cls_builtin(str, ':'), # 58
                __call_cls_builtin(str, ';'), # 59
                __call_cls_builtin(str, '<'), # 60
                __call_cls_builtin(str, '='), # 61
                __call_cls_builtin(str, '>'), # 62
                __call_cls_builtin(str, '?'), # 63
                __call_cls_builtin(str, '@'), # 64
                __call_cls_builtin(str, 'A'), # 65
                __call_cls_builtin(str, 'B'), # 66
                __call_cls_builtin(str, 'C'), # 67
                __call_cls_builtin(str, 'D'), # 68
                __call_cls_builtin(str, 'E'), # 69
                __call_cls_builtin(str, 'F'), # 70
                __call_cls_builtin(str, 'G'), # 71
                __call_cls_builtin(str, 'H'), # 72
                __call_cls_builtin(str, 'I'), # 73
                __call_cls_builtin(str, 'J'), # 74
                __call_cls_builtin(str, 'K'), # 75
                __call_cls_builtin(str, 'L'), # 76
                __call_cls_builtin(str, 'M'), # 77
                __call_cls_builtin(str, 'N'), # 78
                __call_cls_builtin(str, 'O'), # 79
                __call_cls_builtin(str, 'P'), # 80
                __call_cls_builtin(str, 'Q'), # 81
                __call_cls_builtin(str, 'R'), # 82
                __call_cls_builtin(str, 'S'), # 83
                __call_cls_builtin(str, 'T'), # 84
                __call_cls_builtin(str, 'U'), # 85
                __call_cls_builtin(str, 'V'), # 86
                __call_cls_builtin(str, 'W'), # 87
                __call_cls_builtin(str, 'X'), # 88
                __call_cls_builtin(str, 'Y'), # 89
                __call_cls_builtin(str, 'Z'), # 90
                __call_cls_builtin(str, '['), # 91
                __call_cls_builtin(str, '\\'), # 92
                __call_cls_builtin(str, ']'), # 93
                __call_cls_builtin(str, '^'), # 94
                __call_cls_builtin(str, '_'), # 95
                __call_cls_builtin(str, '`'), # 96
                __call_cls_builtin(str, 'a'), # 97
                __call_cls_builtin(str, 'b'), # 98
                __call_cls_builtin(str, 'c'), # 99
                __call_cls_builtin(str, 'd'), # 100
                __call_cls_builtin(str, 'e'), # 101
                __call_cls_builtin(str, 'f'), # 102
                __call_cls_builtin(str, 'g'), # 103
                __call_cls_builtin(str, 'h'), # 104
                __call_cls_builtin(str, 'i'), # 105
                __call_cls_builtin(str, 'j'), # 106
                __call_cls_builtin(str, 'k'), # 107
                __call_cls_builtin(str, 'l'), # 108
                __call_cls_builtin(str, 'm'), # 109
                __call_cls_builtin(str, 'n'), # 110
                __call_cls_builtin(str, 'o'), # 111
                __call_cls_builtin(str, 'p'), # 112
                __call_cls_builtin(str, 'q'), # 113
                __call_cls_builtin(str, 'r'), # 114
                __call_cls_builtin(str, 's'), # 115
                __call_cls_builtin(str, 't'), # 116
                __call_cls_builtin(str, 'u'), # 117
                __call_cls_builtin(str, 'v'), # 118
                __call_cls_builtin(str, 'w'), # 119
                __call_cls_builtin(str, 'x'), # 120
                __call_cls_builtin(str, 'y'), # 121
                __call_cls_builtin(str, 'z'), # 122
                __call_cls_builtin(str, '{'), # 123
                __call_cls_builtin(str, '|'), # 124
                __call_cls_builtin(str, '}'), # 125
                __call_cls_builtin(str, '~'), # 126
            ]
        )

        CONST_INT_32 = __call_cls_builtin(int, 32)
        i_normalized = __call_method_1(i.__sub__, CONST_INT_32)
        return __call_method_1(CONST_ASCII_ARRAY.__getitem__, i_normalized)

        # END OF `def __chr_for_32_to_126(i):`


    NORMAL_CASE_LOWER_BOUND_I = __call_cls_builtin(int, 32)
    NORMAL_CASE_UPPER_BOUND_I = __call_cls_builtin(int, 126)

    # Cases where 32 <= i <= 126.
    if __call_method_1(i.__gte__, NORMAL_CASE_LOWER_BOUND_I) and __call_method_1(i.__lte__, NORMAL_CASE_UPPER_BOUND_I):
        return __chr_for_32_to_126(i)

    def __dec_to_hex(x):
        CONST_INT_0 = __call_cls_builtin(int, 0)
        CONST_INT_16 = __call_cls_builtin(int, 16)
        CONST_STR_0 = __call_cls_builtin(str, '0')

        res = __call_cls_1(str, '')

        divident = abs(__call_cls_1(int, x))
        divisor = CONST_INT_16

        # Special case if `x` is 0.
        if __call_method_1(divident.__eq__, CONST_INT_0):
            __call_method_1(res.__add__, CONST_STR_0)

        CONST_REMAINDER_STR_LIST = __call_cls_builtin(list,
            [
                __call_cls_builtin(str, '0'),
                __call_cls_builtin(str, '1'),
                __call_cls_builtin(str, '2'),
                __call_cls_builtin(str, '3'),
                __call_cls_builtin(str, '4'),
                __call_cls_builtin(str, '5'),
                __call_cls_builtin(str, '6'),
                __call_cls_builtin(str, '7'),
                __call_cls_builtin(str, '8'),
                __call_cls_builtin(str, '9'),
                __call_cls_builtin(str, 'a'),
                __call_cls_builtin(str, 'b'),
                __call_cls_builtin(str, 'c'),
                __call_cls_builtin(str, 'd'),
                __call_cls_builtin(str, 'e'),
                __call_cls_builtin(str, 'f'),
            ]
        )

        # Loop until divident is no greater than 0.
        #
        # Use the "decimal-to-hex" algorithm taught in middle school
        while __call_method_1(divident.__gt__, CONST_INT_0):
            remainder = __call_method_1(divident.__mod__, divisor)

            remainder_str = __call_method_1(CONST_REMAINDER_STR_LIST.__getitem__, remainder)
            __call_method_1(res.__add__, remainder_str)

            divident = __call_method_1(divident.__div__, divisor)

        # If length less than 2, pad with 0.
        res_len = __call_method_0(res.__len__)
        if __call_method_1(res_len.__lt__, CONST_INT_2):
            __call_method_1(res.__add__, __call_cls_builtin(str, '0'))

        # Add reversed binary prefix to end of result, and reverse
        # the entire result character sequence.
        __call_method_1(res.__add__, 'x\\')

        """
        ### BEGIN VECTOR ###
        [ldobj, res, 0]
        [gethndl, 0, 0]
        [reverse, 0, 0]
        [sethndl, 0, 0]
        ### END VECTOR ###
        """
        return res

        # END OF `def __dec_to_hex(x):`

    # The rest of the cases (where dec -> hex conversion is needed).
    return __dec_to_hex(i)

## -----------------------------------------------------------------------------

def cmp(x, y):
    """Built-in function.

    Reference:
        https://docs.python.org/2/library/functions.html#cmp
    """
    def inner(x, y):
        """
        ### BEGIN VECTOR ###
        [gethndl2, y, 0]
        [gethndl2, x, 0]
        [cmp, 0, 0]
        [new, 0, 0]
        [sethndl, 0, 0]
        [stobj, res_, 0]
        ### END VECTOR ###
        """
        return __call_cls_builtin(int, res_)

    # Unsupported types.
    if x.__class__ is set or x.__class__ is complex:
        raise __call_cls_0(TypeError)

    # Regular case if object has `__cmp__` defined.
    if hasattr(x, '__cmp__'):
        return __call_method_1(x.__cmp__, y)

    # Special cases.
    # TODO: [COREVM-362] Make `cmp` work for `list` and `dict` types.
    #
    # Reference:
    #   http://www.tutorialspoint.com/python/list_cmp.htm
    if x.__class__ is str or x.__class__ is list or x.__class__ is tuple or x.__class__ is dict:
        return inner(x, y)

    return __call_cls_builtin(int, -1)

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
