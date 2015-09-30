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
    elif hasattr(caller, '__call__'):
        return caller.__call__(caller, *args, **kwargs)
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

    # TODO: Add support for Python long integer type.
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

        # Add reversed hexadecimal prefix to end of result, and reverse
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

def divmod(a, b):
    """Built-in function.

    Reference:
        https://docs.python.org/2/library/functions.html#divmod
    """
    # TODO: Add support for `complex` type.
    # [COREVM-363] Add support for other attributes for Python `complex` type
    if a.__class__ is float and b.__class__ is int:
        b = __call_cls_1(float, b)

    if a.__class__ is int and b.__class__ is float:
        a = __call_cls_1(float, a)

    if hasattr(a, '__divmod__'):
        return __call_method_1(a.__divmod__, b)

    raise __call_cls_0(TypeError)

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

def filter(function_, iterable):
    """Built-in function.

    Reference:
        https://docs.python.org/2/library/functions.html#filter
    """
    iterator_ = __call_method_0(iterable.__iter__)

    if iterable.__class__ is str:
        res = __call_cls_builtin(str, '')
        modifier = res.__add__
    else:
        res = __call_cls_builtin(list, [])
        modifier = res.append

    func = function_ if function_ is not None else lambda x: x

    try:
        while True:
            item = __call_method_0(iterator_.next)

            if __call_method_0(func(item).__nonzero__):
                __call_method_1(modifier, item)
    except StopIteration:
        pass

    if iterable.__class__ is tuple:
        res = __call_cls_builtin(tuple, res)

    return res

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

def hash(obj):
    """Built-in function.

    Reference:
        https://docs.python.org/2/library/functions.html#hash
    """

    if hasattr(obj, '__hash__'):
        return __call_method_0(obj.__hash__)

    """
    ### BEGIN VECTOR ###
    [ldobj, obj, 0]
    [putobj, 0, 0]
    [hash, 0, 0]
    [new, 0, 0]
    [sethndl, 0, 0]
    [stobj, res_, 0]
    ### END VECTOR ###
    """
    return __call_cls_builtin(int, res_)

## -----------------------------------------------------------------------------

def hex(x):
    """Built-in function.

    Reference:
        https://docs.python.org/2/library/functions.html#hex
    """
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

        # Add reversed hexadecimal prefix to end of result.
        __call_method_1(res.__add__, 'x0')

        # Add negative sign if original parameter is negative.
        if __call_method_1(x.__lt__, CONST_INT_0):
            __call_method_1(res.__add__, '-')

        # Reverse entire result character sequence.
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

    # TODO: Add support for Python long integer type.
    if x.__class__ is int or x.__class__ is bool:
        return __dec_to_hex(x)

    # The spec specifies that if the object is not of integer type,
    # it has to define the `__index__` method in order for `hex` to work.
    # However, this contradicts with the behavior from running Python v2.7.6.
    # It seems like Python's behavior is still supporting the old rule
    # for overloading `hex` with defining the `__hex__` method on an object,
    # but not supporting the `__index__` overloading rule in the spec.
    #if hasattr(x, '__index__'):
    #   return __dec_to_hex(__call_method_0(x.__index__))

    if hasattr(x, '__hex__'):
        return __call_method_0(x.__hex__)

    raise __call_cls_0(TypeError)

## -----------------------------------------------------------------------------

def id(obj):
    """Built-in function.

    Reference:
        https://docs.python.org/2/library/functions.html#id
    """

    """
    ### BEGIN VECTOR ###
    [ldobj, obj, 0]
    [putobj, 0, 0]
    [new, 0, 0]
    [sethndl, 0, 0]
    [stobj, res_, 0]
    ### END VECTOR ###
    """
    return __call_cls_1(int, res_)

## -----------------------------------------------------------------------------

def iter(o):
    """Built-in function.

    Reference:
        https://docs.python.org/2/library/functions.html#iter
    """

    # Capture non-keyword optional argument `sentinel`.
    """
    ### BEGIN VECTOR ###
    [hasargs, 0, 0]
    [jmpif, 3, 0]

    [ldobj, None, 0]
    [stobj, sentinel, 0]
    [jmp, 2, 0]

    [getarg, 0, 0]
    [stobj, sentinel, 0]
    ### END VECTOR ###
    """

    class defaultiterator(object):

        def __init__(self, iterable):
            self.iterable_iter = __call_method_0(iterable.__iter__)

        def next(self):
            return __call_method_0(self.iterable_iter.next)

    class sequenceiterator(object):

        def __init__(self, iterable):
            self.iterable = iterable
            self.i = __call_cls_builtin(int, 0)
            self.n = __call_method_0(iterable.__len__)
            self.CONST_INT_1 = __call_cls_builtin(int, 1)

        def next(self):
            if __call_method_1(self.i.__lt__, self.n):
                res = __call_method_1(self.iterable.__getitem__, self.i)
                __call_method_1(self.i.__iadd__, self.CONST_INT_1)
                return res
            else:
                raise __call_cls_0(StopIteration)

    class callableiterator(object):

        def __init__(self, iterable, sentinel):
            self.iterable = iterable
            self.sentinel = sentinel

        def next(self):
            res = __call(self.iterable)

            if __call_method_1(res.__eq__, self.sentinel):
                raise __call_cls_0(StopIteration)

            return res

    if sentinel is None:
        if hasattr(o, '__iter__'):
            return __call_cls_1(defaultiterator, o)
        elif hasattr(o, '__getitem__'):
            return __call_cls_1(sequenceiterator, o)
        else:
            raise __call_cls_0(TypeError)
    else:
        return __call_cls_2(callableiterator, o, sentinel)

## -----------------------------------------------------------------------------

def len(s):
    """Built-in function.

    Reference:
        https://docs.python.org/2/library/functions.html#len
    """
    if hasattr(s, '__len__'):
        return __call_method_0(s.__len__)

    raise __call_cls_0(TypeError)

## -----------------------------------------------------------------------------

def map(function, iterable, *iterables):
    """Built-in function.

    Reference:
        https://docs.python.org/2/library/functions.html#map
    """

    # TODO: handle the case where `function` is None and fall back to
    # use the identity function. Need support for variable unpacking.
    #
    # [COREVM-315] Add support for variable unpacking

    iterables = __call_cls_builtin(list, iterables)

    CONST_INT_1 = __call_cls_builtin(int, 1)

    num_iterables = __call_method_0(iterables.__len__)
    __call_method_1(num_iterables.__iadd__, CONST_INT_1)

    max_len = len(iterable)

    iterables_iter = __call_method_0(iterables.__iter__)

    try:
        while True:
            iterable_ = __call_method_0(iterables_iter.next)

            if hasattr(iterable_, '__getitem__') or hasattr(iterable_, '__iter__'):
                max_len = max(max_len, len(iterable_))
            else:
                raise __call_cls_0(TypeError)
    except StopIteration:
        pass
    finally:
        # Let the inner `TypeError` bubble up.
        pass

    def getitem(iterable, i):
        if __call_method_1(i.__lt__, __call_method_0(iterable.__len__)):
            return __call_method_1(iterable.__getitem__, i)
        return None

    i = __call_cls_builtin(int, 0)
    res = __call_cls_builtin(list, [])

    while __call_method_1(i.__lt__, max_len):
        args = __call_cls_1(list, [])

        __call_method_1(args.append, getitem(iterable, i))

        iterables_iter = __call_method_0(iterables.__iter__)

        # Get args from iterables.
        try:
            while True:
                iterable_ = __call_method_0(iterables_iter.next)
                __call_method_1(args.append, getitem(iterable_, i))
        except StopIteration:
            pass

        res_val = __call(function, *args)

        __call_method_1(res.append, res_val)

        __call_method_1(i.__iadd__, CONST_INT_1)

    return res

## -----------------------------------------------------------------------------

def next(iterator):
    """Built-in function.

    Reference:
        https://docs.python.org/2/library/functions.html#next
    """

    # Capture non-keyword optional argument `default`.
    """
    ### BEGIN VECTOR ###
    [hasargs, 0, 0]
    [truthy, 0, 0]
    [new, 0, 0]
    [sethndl, 0, 0]
    [stobj, has_default, 0]
    [jmpif, 3, 0]

    [ldobj, None, 0]
    [stobj, default, 0]
    [jmp, 2, 0]

    [getarg, 0, 0]
    [stobj, default, 0]
    ### END VECTOR ###
    """

    has_default = __call_cls_builtin(bool, has_default)

    if __call_method_0(hasattr(iterator, 'next').__not__):
        raise __call_cls_0(TypeError)

    try:
        return __call_method_0(iterator.next)
    except StopIteration as ex:
        if has_default:
            return default
        else:
            raise ex

## -----------------------------------------------------------------------------

def oct(x):
    """Built-in function.

    Reference:
        https://docs.python.org/2/library/functions.html#oct
    """

    def __dec_to_oct(x):
        CONST_INT_0 = __call_cls_builtin(int, 0)
        CONST_INT_8 = __call_cls_builtin(int, 8)
        CONST_STR_0 = __call_cls_builtin(str, '0')

        res = __call_cls_1(str, '')

        divident = abs(__call_cls_1(int, x))
        divisor = CONST_INT_8

        # Loop until divident is no greater than 0.
        #
        # Use the "decimal-to-binary" algorithm taught in middle school
        # http://www.wikihow.com/Convert-from-Decimal-to-Binary
        while __call_method_1(divident.__gt__, CONST_INT_0):
            remainder = __call_method_1(divident.__mod__, divisor)

            __call_method_1(res.__add__, __call_method_0(remainder.__repr__))

            divident = __call_method_1(divident.__div__, divisor)

        # Leading zero.
        # (CPython always have leading zero in front of all oct numbers).
        __call_method_1(res.__add__, CONST_STR_0)

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

        # END of `__dec_to_oct(x)`

    if x.__class__ is int or x.__class__ is bool:
        return __dec_to_oct(x)

    if hasattr(x, '__oct__'):
        return __call_method_0(x.__oct__)

    raise __call_cls_0(TypeError)

## -----------------------------------------------------------------------------

def pow(x, y):
    """Built-in function.

    Reference:
        https://docs.python.org/2/library/functions.html#pow
    """

    # Capture non-keyword optional argument `z`.
    """
    ### BEGIN VECTOR ###
    [hasargs, 0, 0]
    [jmpif, 3, 0]

    [ldobj, None, 0]
    [stobj, z, 0]
    [jmp, 2, 0]

    [getarg, 0, 0]
    [stobj, z, 0]
    ### END VECTOR ###
    """

    x = __call_cls_1(x.__class__, x)
    y = __call_cls_1(y.__class__, y)

    CONST_INT_0 = __call_cls_builtin(int, 0)

    if z is not None:
        # If `z` is present, `x` and `y` must be of integer types,
        # and `y` must be non-negative.
        if x.__class__ is not int and x.__class__ is not bool:
            raise __call_cls_0(TypeError)

        if y.__class__ is not int and y.__class__ is not bool:
            raise __call_cls_0(TypeError)

        # Apparently `z` has to be integer as well.
        if z.__class__ is not int and z.__class__ is not bool:
            raise __call_cls_0(TypeError)

        if __call_method_1(y.__lt__, CONST_INT_0):
            raise __call_cls_0(TypeError)

        if __call_method_1(z.__eq__, CONST_INT_0):
            raise __call_cls_0(ValueError)
    else:
        # If `z` is not present, `y` could be negative, and if that's the case,
        # then both operands must be converted into floats.
        if __call_method_1(y.__lt__, CONST_INT_0):
            x = __call_cls_1(float, x)
            y = __call_cls_1(float, y)

            # 0.0 cannot be raised to a negative power
            if __call_method_1(x.__eq__, CONST_INT_0):
                raise __call_cls_0(ZeroDivisionError)

    # If either operand is of type `float`, the other one has to be as well
    # in order for the result to be of type `float`.
    if x.__class__ is float and y.__class__ is not float:
        y = __call_cls_1(float, y)
    elif y.__class__ is float and x.__class__ is not float:
        x = __call_cls_1(float, x)

    res = __call_method_1(x.__pow__, y)

    if z is not None:
        res = __call_method_1(res.__mod__, z)

    return res

## -----------------------------------------------------------------------------

def range(arg):
    """Built-in function.

    Reference:
        https://docs.python.org/2/library/functions.html#range
    """

    # Capture non-keyword optional argument `arg2`.
    """
    ### BEGIN VECTOR ###
    [hasargs, 0, 0]
    [truthy, 0, 0]
    [new, 0, 0]
    [sethndl, 0, 0]
    [stobj, has_arg2, 0]
    [jmpif, 3, 0]

    [ldobj, None, 0]
    [stobj, arg2, 0]
    [jmp, 2, 0]

    [getarg, 0, 0]
    [stobj, arg2, 0]
    ### END VECTOR ###
    """

    # Capture non-keyword optional argument `step`.
    """
    ### BEGIN VECTOR ###
    [hasargs, 0, 0]
    [truthy, 0, 0]
    [new, 0, 0]
    [sethndl, 0, 0]
    [stobj, has_step, 0]
    [jmpif, 3, 0]

    [ldobj, None, 0]
    [stobj, step, 0]
    [jmp, 2, 0]

    [getarg, 0, 0]
    [stobj, step, 0]
    ### END VECTOR ###
    """

    has_arg2 = __call_cls_builtin(bool, has_arg2)
    has_step = __call_cls_builtin(bool, has_step)

    if arg.__class__ is not int and arg.__class__ is not bool:
        raise __call_cls_0(TypeError)

    CONST_INT_0 = __call_cls_1(int, 0)

    if has_arg2:
        # Both argument are specified.
        if arg2.__class__ is not int and arg2.__class__ is not bool:
            raise __call_cls_0(TypeError)

        start = arg
        stop = arg2
    else:
        # If only one argument is specified.
        stop = __call_cls_1(int, arg)
        start = __call_cls_1(int, 0)

    res = __call_cls_builtin(list, [])

    ascending = __call_cls_1(bool, True)

    # `step`.
    if __call_method_0(has_step.__not__):
        step = __call_cls_1(int, 1)
    elif step.__class__ is not int and step.__class__ is not bool:
        raise __call_cls_0(TypeError)
    elif __call_method_1(step.__eq__, CONST_INT_0):
        raise __call_cls_0(ValueError)
    else:
        ascending = __call_method_1(__call_method_1(stop.__sub__, start).__gt__, CONST_INT_0)
        positive_step = __call_method_1(step.__gt__, CONST_INT_0)
        if __call_method_1(ascending.__ne__, positive_step):
            return res

    cumulated = __call_cls_builtin(int, start)

    comparer = cumulated.__lt__ if ascending else cumulated.__gt__

    while comparer(cumulated, stop):
        __call_method_1(res.append, __call_cls_1(int, cumulated))
        __call_method_1(cumulated.__iadd__, step)

    return res

## -----------------------------------------------------------------------------

def reduce(function, iterable):
    """Built-in function.

    Reference:
        https://docs.python.org/2/library/functions.html#reduce
    """

    # Capture non-keyword optional argument `initializer`.
    """
    ### BEGIN VECTOR ###
    [hasargs, 0, 0]
    [jmpif, 3, 0]

    [ldobj, None, 0]
    [stobj, initializer, 0]
    [jmp, 2, 0]

    [getarg, 0, 0]
    [stobj, initializer, 0]
    ### END VECTOR ###
    """

    it = iter(iterable)
    if initializer is None:
        try:
            initializer = next(it)
        except StopIteration:
            raise __call_cls_0(TypeError)

    accum_value = initializer

    try:
        while True:
            x = next(it)
            accum_value = function(accum_value, x)
    except StopIteration:
        pass

    return accum_value

## -----------------------------------------------------------------------------

def reversed(seq):
    """Built-in function.

    Reference:
        https://docs.python.org/2/library/functions.html#reversed
    """

    if hasattr(seq, '__reversed__'):
        return __call_method_0(seq.__reversed__)

    has_len = hasattr(seq, '__len__')
    has_getitem = hasattr(seq, '__getitem__')

    if __call_method_0(has_len.__not__) or __call_method_0(has_getitem.__not__):
        raise __call_cls_0(TypeError)

    CONST_INT_0 = __call_cls_builtin(int, 0)
    CONST_INT_1 = __call_cls_builtin(int, 1)

    class reversed(object):

        def __init__(self, iterable_):
            self.iterable = iterable_
            self.n = __call_method_0(iterable_.__len__)
            self.i = __call_method_1(self.n.__sub__, CONST_INT_1)

        def next(self):
            if __call_method_1(self.i.__gte__, CONST_INT_0):
                res = __call_method_1(self.iterable.__getitem__, self.i)
                __call_method_1(self.i.__isub__, CONST_INT_1)
                return res
            else:
                raise __call_cls_0(StopIteration)

    return __call_cls_1(reversed, seq)

## -----------------------------------------------------------------------------

def round(number):
    """Built-in function.

    Reference:
        https://docs.python.org/2/library/functions.html#round
    """

    if number.__class__ is not float and number.__class__ is not int and number.__class__ is not bool:
        raise __call_cls_0(TypeError)

    # Capture non-keyword optional argument `ndigits`.
    """
    ### BEGIN VECTOR ###
    [hasargs, 0, 0]
    [jmpif, 3, 0]

    [ldobj, None, 0]
    [stobj, ndigits, 0]
    [jmp, 2, 0]

    [getarg, 0, 0]
    [stobj, ndigits, 0]
    ### END VECTOR ###
    """

    CONST_INT_0 = __call_cls_builtin(int, 0)

    if ndigits is None:
        ndigits = __call_cls_builtin(int, CONST_INT_0)
    elif ndigits.__class__ is not int and ndigits.__class__ is not bool:
        raise __call_cls_0(TypeError)

    if __call_method_1(ndigits.__lt__, CONST_INT_0):
        return __call_cls_1(float, CONST_INT_0)

    number_ = __call_cls_1(float, number)

    """
    ### BEGIN VECTOR ###
    [gethndl2, number_, 0]
    [gethndl2, ndigits, 0]
    [round, 0, 0]
    [new, 0, 0]
    [sethndl, 0, 0]
    [stobj, res_, 0]
    ### END VECTOR ###
    """

    return __call_cls_1(float, res_)

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

def sorted(iterable, key=None, reverse=False):
    """Built-in function.

    Reference:
        https://docs.python.org/2/library/functions.html#sorted
    """

    has_len = hasattr(iterable, '__len__')
    has_getitem = hasattr(iterable, '__getitem__')
    if __call_method_0(has_len.__not__) or __call_method_0(has_getitem.__not__):
        raise __call_cls_0(TypeError)

    # Capture non-keyword optional argument `cmp`.
    """
    ### BEGIN VECTOR ###
    [hasargs, 0, 0]
    [jmpif, 1, 0]

    [jmp, 2, 0]

    [getarg, 0, 0]
    [stobj, cmp, 0]
    ### END VECTOR ###
    """

    # Sorting algorithm:
    #
    # for i = 1 to length(A) - 1
    #   x = A[i]
    #   j = i
    #   while j > 0 and A[j-1] > x
    #     A[j] = A[j-1]
    #     j = j - 1
    #   end while
    #
    #   A[j] = x
    # end for

    if key is None:
        key = lambda x: x

    if reverse:
        cmp_ = cmp
        cmp = lambda x, y: cmp_(y, x)

    iterable = __call_cls_1(iterable.__class__, iterable)
    iterable_by_key = map(key, iterable)

    i = __call_cls_1(int, 1)
    len_ = __call_method_0(iterable.__len__)

    while __call_method_1(i.__lt__, len_):
        x = __call_method_1(iterable.__getitem__, i)
        x_by_key = __call_method_1(iterable_by_key.__getitem__, i)
        j = __call_cls_1(int, i)

        while __call_method_1(j.__gt__, 0):
            j_1 = __call_method_1(j.__sub__, 1)
            if __call_method_1(__call(cmp, __call_method_1(iterable_by_key.__getitem__, j_1), x_by_key).__eq__, 1):
                tmp = __call_method_1(iterable_by_key.__getitem__, j_1)
                __call_method_2(iterable_by_key.__setitem__, j, tmp)

                tmp = __call_method_1(iterable.__getitem__, j_1)
                __call_method_2(iterable.__setitem__, j, tmp)
                j = __call_method_1(j.__sub__, 1)
            else:
                break

        __call_method_2(iterable.__setitem__, j, x)
        __call_method_2(iterable_by_key.__setitem__, j, x_by_key)

        i = __call_method_1(i.__add__, 1)

    return iterable

## -----------------------------------------------------------------------------

def sum(iterable):
    """Built-in function.

    Reference:
        https://docs.python.org/2/library/functions.html#sum
    """

    # Capture non-keyword optional argument `start`.
    """
    ### BEGIN VECTOR ###
    [hasargs, 0, 0]
    [jmpif, 3, 0]

    [ldobj, None, 0]
    [stobj, start, 0]
    [jmp, 2, 0]

    [getarg, 0, 0]
    [stobj, start, 0]
    ### END VECTOR ###
    """

    if start is None:
        start = __call_cls_1(int, 0)
    elif start.__class__ is str:
        raise __call_cls_0(TypeError)

    iterator_ = iter(iterable)

    res = __call_cls_1(start.__class__, start)

    try:
        while True:
            item = next(iterator_)

            res = __call_method_1(res.__add__, item)
    except StopIteration:
        pass

    return res

## -----------------------------------------------------------------------------

def zip(*iterables):
    """Built-in function.

    Reference:
        https://docs.python.org/2/library/functions.html#zip
    """

    # NOTE: According to the Python reference above,
    # `zip()` is similar to `map()` with an initial argument of `None`.
    # However, we cannot use `map()` here because `map()` expands iterables
    # in the arguments to match the longest iterable, whereas the behavior
    # of `zip()` returns a list with the size that matches with the length
    # of the shortest iterable in the arguments.

    iterables = __call_cls_builtin(list, iterables)
    num_iterables = __call_method_0(iterables.__len__)

    if __call_method_1(num_iterables.__eq__, 0):
        return __call_cls_1(list, [])

    CONST_INT_1 = __call_cls_builtin(int, 1)

    min_len = __call_cls_1(int, 4294967295)

    iterables_iter = __call_method_0(iterables.__iter__)

    try:
        while True:
            iterable_ = __call_method_0(iterables_iter.next)

            if hasattr(iterable_, '__getitem__') or hasattr(iterable_, '__iter__'):
                min_len = min(min_len, len(iterable_))
            else:
                raise __call_cls_0(TypeError)
    except StopIteration:
        pass
    finally:
        # Let the inner `TypeError` bubble up.
        pass

    def getitem(iterable, i):
        if __call_method_1(i.__lt__, __call_method_0(iterable.__len__)):
            return __call_method_1(iterable.__getitem__, i)
        return None

    i = __call_cls_builtin(int, 0)
    res = __call_cls_builtin(list, [])

    while __call_method_1(i.__lt__, min_len):
        res_element = __call_cls_1(list, [])

        iterables_iter = __call_method_0(iterables.__iter__)

        try:
            while True:
                iterable_ = __call_method_0(iterables_iter.next)
                __call_method_1(res_element.append, getitem(iterable_, i))
        except StopIteration:
            pass

        res_element = __call_cls_1(tuple, res_element)

        __call_method_1(res.append, res_element)

        __call_method_1(i.__iadd__, CONST_INT_1)

    return res

## -----------------------------------------------------------------------------
