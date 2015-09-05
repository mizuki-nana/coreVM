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

CONST_STR_SLICE = __call_cls_builtin(str, 'slice(')
CONST_STR_COMMA = __call_cls_builtin(str, ', ')
CONST_STR_CLOSE_PAREN  = __call_cls_builtin(str, ')')
CONST_INT_ZERO = __call_cls_builtin(int, 0)

## -----------------------------------------------------------------------------

class slice(object):

    def __init__(self, start, stop, step):
        self.start = start
        self.stop = stop
        self.step = step

    def __str__(self):
        res = __call_cls_builtin(str, '')

        __call_method_1(res.__add__, CONST_STR_SLICE)
        __call_method_1(res.__add__, __call_method_0(self.start.__repr__))
        __call_method_1(res.__add__, CONST_STR_COMMA)
        __call_method_1(res.__add__, __call_method_0(self.stop.__repr__))
        __call_method_1(res.__add__, CONST_STR_COMMA)
        __call_method_1(res.__add__, __call_method_0(self.step.__repr__))
        __call_method_1(res.__add__, CONST_STR_CLOSE_PAREN)

        return res

    def __repr__(self):
        return __call_method_0(self.__str__)

    def __hash__(self):
        raise __call_cls_0(TypeError)

    def indices(self, num):
        start = min(self.start, num)
        stop = min(self.stop, num)
        step = __call_cls_1(int, self.step)

        if __call_method_1(step.__eq__, CONST_INT_ZERO):
            raise __call_cls_0(ValueError)

        res = __call_cls_builtin(tuple, (start, stop, step))

        return res

## -----------------------------------------------------------------------------
