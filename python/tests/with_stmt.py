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

class DummyContextManager(object):

    def __init__(self):
        pass

    def __enter__(self):
        print 'DummyContextManager.__enter__'

    def __exit__(self, exc_type, exc_val, exc_tb):
        print 'DummyContextManager.__exit__'
        return True

## -----------------------------------------------------------------------------

def test_dummy_context_manager():
    with DummyContextManager():
        print 'Inside body'

## -----------------------------------------------------------------------------

def test_dummy_context_manager_with_exception():
    with DummyContextManager():
        raise Exception()

## -----------------------------------------------------------------------------

class NumberContextManager(object):

    def __init__(self, n):
        self.n = n

    def __enter__(self):
        self.n += 1
        return self.n

    def __exit__(self, exc_type, exc_val, exc_tb):
        pass

## -----------------------------------------------------------------------------

def test_number_context_manager():
    with NumberContextManager(1) as n:
        print n

## -----------------------------------------------------------------------------

class DivisionContextManager(object):

    def __init__(self, raise_exception):
        self.raise_exception = raise_exception

    def __enter__(self):
        return self

    def calculate(self, numerator, denominator):
        if denominator == 0:
            return None

        return numerator / denominator

    def __exit__(self, exc_type, exc_val, exc_tb):
        # TODO: [COREVM-297]_Python if-stmt-else-stmt construct executes both conditions
        if exc_val is None:
            print 'Division successful'
        else:
            print 'Error in division'

        return self.raise_exception

## -----------------------------------------------------------------------------

def test_division_context_manager_with_valid_inputs_and_raise_exception():
    try:
      with DivisionContextManager(True) as ctx:
          res = ctx.calculate(4, 2)
          if res is None:
              raise Exception()
    except Exception:
        print 'Division should not raise an exception'

## -----------------------------------------------------------------------------

def test_division_context_manager_with_invalid_inputs_and_raise_exception():
    try:
        with DivisionContextManager(True) as ctx:
            res = ctx.calculate(2, 0)
            if res is None:
                raise Exception()
    except Exception:
        print 'Division exception caught'

## -----------------------------------------------------------------------------

def test_division_context_manager_with_valid_inputs_and_no_raise_exception():
    try:
      with DivisionContextManager(False) as ctx:
          res = ctx.calculate(4, 2)
          if res is None:
              raise Exception()
    except Exception:
        print 'Division should not raise an exception'

## -----------------------------------------------------------------------------

def test_division_context_manager_with_invalid_inputs_and_no_raise_exception():
    try:
        with DivisionContextManager(False) as ctx:
            res = ctx.calculate(2, 0)
            if res is None:
                raise Exception()
    except Exception:
        print 'Division should raise an uncaught exception'

## -----------------------------------------------------------------------------

test_dummy_context_manager()
test_dummy_context_manager_with_exception()
test_number_context_manager()
test_division_context_manager_with_valid_inputs_and_raise_exception()
test_division_context_manager_with_invalid_inputs_and_raise_exception()
test_division_context_manager_with_valid_inputs_and_no_raise_exception()
test_division_context_manager_with_invalid_inputs_and_no_raise_exception()

## -----------------------------------------------------------------------------
