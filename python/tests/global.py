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


# Tests the `global` keyword.
# Executes the following code in series.

## -----------------------------------------------------------------------------

def assign_global_in_func():
    global x, y
    x = 3
    y = 4
    print 'In inner'
    print x, y

assign_global_in_func()
print 'In outer'
print x, y

## -----------------------------------------------------------------------------

def aug_assign_in_inner_func():
    def inner():
        global x
        x += 2

    inner()

aug_assign_in_inner_func()
print x

## -----------------------------------------------------------------------------

def assign_global_in_inner_func():
    def inner():
        global z
        z = 'Why why why'

    inner()

assign_global_in_inner_func()
print z

## -----------------------------------------------------------------------------

def assign_global_for_function():
    global hello_world_func
    def hello_world_func():
        print 'Hello world'

assign_global_for_function()
hello_world_func()

## -----------------------------------------------------------------------------

def assign_global_for_lambda():
    global lam
    lam = lambda x: x**2

assign_global_for_lambda()
print lam(5)

## -----------------------------------------------------------------------------

def assign_global_for_class():
    global MyObject

    class MyObject(object):

        def __init__(self):
            pass

        def to_string(self):
            print 'I am an object'

assign_global_for_class()
MyObject().to_string()

## -----------------------------------------------------------------------------

def assign_global_for_function_in_inner_func():
    def inner():
        global tell_a_story
        def tell_a_story():
            print 'Once upon a time...'

    inner()

assign_global_for_function_in_inner_func()
hello_world_func()

## -----------------------------------------------------------------------------

def assign_global_for_lambda_in_inner_func():
    def inner():
        global cubic
        cubic = lambda x: x ** 3

    inner()

assign_global_for_lambda_in_inner_func()
print cubic(8)

## -----------------------------------------------------------------------------

def assign_global_for_class_in_inner_func():
    def inner():
        global GodObject

        class GodObject(object):

            def __init__(self):
                pass

            def to_string(self):
                print 'I am an instance of god'

    inner()

assign_global_for_class_in_inner_func()
GodObject().to_string()

## -----------------------------------------------------------------------------
