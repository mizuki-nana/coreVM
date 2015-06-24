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

def do_math(arg):
    print 'Time to have fun!'
    return int(5)

def hello_world_again(arg):
    print 'Hello world again!'

def introduction(arg):
    print 'My name is Will :-)'
    # test explicit return should work.
    return

def hello_world(arg):
    print 'Hello world!'
    introduction(arg)

def main(arg):
    print 'Hi'
    hello_world(arg)
    hello_world_again(do_math(arg))
    hello_world(arg)
    print 'Bye'

print 'Starting...'
main(int(1))
print 'Done!'

## -----------------------------------------------------------------------------

func = lambda x: x * 2

print func(1)

## -----------------------------------------------------------------------------

def lambda_caller(arg):
    def inner(arg):
        func = lambda arg: arg * arg
        return func(arg)
    return inner(arg)

print lambda_caller(3)

## -----------------------------------------------------------------------------

def lambda_caller2():
    def inner2(arg):
        func = lambda arg: arg ** arg
        return func(arg)
    return inner2

print lambda_caller2()(5)

## -----------------------------------------------------------------------------

def lambda_caller3():
    def inner3(arg):
        def inner4(arg):
            func = lambda arg: arg ** arg
            return func(arg)
        return inner4(arg) * arg
    return inner3

print lambda_caller3()(5)

## -----------------------------------------------------------------------------

def test_Fibonacci():
    def fibonacci(n):
        if n == 0:
            return 0
        elif n == 1:
            return 1
        else:
            return fibonacci(n - 1) + fibonacci(n - 2)

    print fibonacci(0)
    print fibonacci(1)
    print fibonacci(2)
    print fibonacci(10)

## -----------------------------------------------------------------------------

test_Fibonacci()

## -----------------------------------------------------------------------------
