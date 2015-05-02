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

    def __new__(cls, *arg):
        """
        ### BEGIN VECTOR ###
        [new, 0, 0]
        [ldobj, cls, 0]
        [setattr, __class__, 0]
        [ldobj, cls, 0]
        [setattrs, 1, 1]
        [rsetattrs, im_self, 0]
        ### END VECTOR ###
        """

## -----------------------------------------------------------------------------

def __call(caller, *arg):
    # Need to support *args and **kwargs.
    if caller.__class__ is type:
        obj = object.__new__(caller, *arg)
        caller.__init__(obj, *arg)
        return obj
    elif caller.__class__ is MethodType:
        return caller(caller.im_self, *arg)
    else:
        return caller(*arg)

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
        iterator = __call(self.iterable.__iter__)

        try:
            while True:
                next_item = __call(iterator.next)

                # TODO: associate types with lambdas, so that we can do
                # None check on them.
                okay = True
                if self.predicate is not None:
                    okay = __call(self.predicate, next_item)

                if okay:
                    item = __call(self.elt, next_item)

                    __call(self.synthesizer, self.res, item)
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
                key_value_pair = __call(iterator.next)

                key = key_value_pair.key
                value = key_value_pair.value

                # TODO: associate types with lambdas, so that we can do
                # None check on them.
                okay = True
                if self.predicate is not None:
                    okay = __call(self.predicate, key, value)

                if okay:
                    item = __call(self.elt, key, value)

                    __call(self.synthesizer, self.res, item.key, item.value)
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
        iterator = __call(self.iterable.__iter__)

        try:
            while True:
                next_item = __call(iterator.next)

                # TODO: associate types with lambdas, so that we can do
                # None check on them.
                okay = True
                if self.predicate is not None:
                    okay = __call(self.predicate, next_item)

                if okay:
                    item = __call(self.elt, next_item)

                    __call(self.synthesizer, self.res, item.key, item.value)
        except StopIteration:
            pass

        return self.res

## -----------------------------------------------------------------------------
