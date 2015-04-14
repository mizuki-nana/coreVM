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

# TODO: Run this test when inheritance is supported.
#try:
#    raise YetAnotherException()
#except AnotherException():
#    print 'This should have not be printed'

## -----------------------------------------------------------------------------
