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

class AnotherException(Exception):

    def __init__(self):
        pass


class YetAnotherException(Exception):

    def __init__(self):
        pass

    def __str__(self):
        return 'Yet another exception'

## -----------------------------------------------------------------------------

try:
    raise Exception()
except Exception as exc:
    print 'Everything is going to be okay'

## -----------------------------------------------------------------------------

try:
    raise YetAnotherException()
except AnotherException:
    print 'This is not it either'
except YetAnotherException as exc:
    print 'Found it'
    print exc

## -----------------------------------------------------------------------------

def hello_world():
    raise Exception()

try:
    hello_world()
except Exception:
    print 'Catching exception from another function'

## -----------------------------------------------------------------------------

def greetings():
    hello_world()

try:
    greetings()
except Exception:
    print 'Catching exception from nested function'

## -----------------------------------------------------------------------------

def catch_exception_from_same_level_of_except_block():
    try:
        raise AnotherException()
    except AnotherException:
        raise YetAnotherException()
    except YetAnotherException:
        print 'Catching exception from same level of except blocks is not supported in Python'

try:
    catch_exception_from_same_level_of_except_block()
except YetAnotherException:
    print 'Catching exception from exception raised in except block'

## -----------------------------------------------------------------------------

def welcome():
    greetings()

try:
    welcome()
except Exception:
    print 'Catching exception through triple calls'

## -----------------------------------------------------------------------------

def explode():
    raise Exception()

def handle_explosion():
    try:
        explode()
    except Exception:
        raise AnotherException()

def do_something():
    try:
        handle_explosion()
    except AnotherException:
        raise YetAnotherException()

def run():
    try:
        do_something()
    except YetAnotherException:
        print 'Catching triple-nested exception is fun!'

## -----------------------------------------------------------------------------

def handle_explosion2():
    try:
        explode()
    except Exception:
        print 'Explosion handled'

    # Do some math to calm down after handling an explosion.
    i = 1 + 2

    explode()

def do_something_again():
    try:
        handle_explosion2()
    except Exception:
        print 'Explosion handled again'

## -----------------------------------------------------------------------------

def exception_in_lambda():
    def inner():
        raise AnotherException()

    inner()

try:
    exception_in_lambda()
except AnotherException:
    print 'Catching exception from lambda'

## -----------------------------------------------------------------------------

def parent():
    def inner2():
        raise Exception()

    return inner2

try:
    parent()()
except Exception:
    print 'Exception from nested function caught.'

## -----------------------------------------------------------------------------

try:
    print 'Do something here...'
except YetAnotherException:
    print 'This is not the right exception'
else:
    print 'This should be printed'

## -----------------------------------------------------------------------------

try:
    raise AnotherException()
except AnotherException:
    print 'This should be printed'
else:
    print 'This should not be printed'

## -----------------------------------------------------------------------------

def raise_another_exception():
    raise AnotherException()

try:
    raise_another_exception()
except AnotherException:
    print 'AnotherException caught'
else:
    print 'This should not be printed'

## -----------------------------------------------------------------------------

# TODO: [COREVM-275] Python compiler elide closure of previously-named function
def run2():
    parent()()

try:
    run2()
except:
    print 'Exception caught'
else:
    print 'This should not be printed'

## -----------------------------------------------------------------------------

# TODO: Run this test when inheritance is supported.
#try:
#    raise YetAnotherException()
#except AnotherException():
#    print 'This should have not be printed'

## -----------------------------------------------------------------------------
