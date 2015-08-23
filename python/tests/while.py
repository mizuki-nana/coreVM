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

def test_while_loop():
    i = 10

    while i:
        print 'hello world'
        i = i - 1

## -----------------------------------------------------------------------------

def test_while_loop_with_continue():
    i = 10
    while i:
        i = i - 1
        continue
        print 'This can never be said...'

## -----------------------------------------------------------------------------

def test_while_loop_with_continue_2():
    i = 1
    while i <= 5:
        i += 1

        if i % 2 == 0:
            continue
        else:
            print i

## -----------------------------------------------------------------------------

def test_while_loop_with_break():
    i = 10
    while i:
        break
        print 'This can never be said...'

## -----------------------------------------------------------------------------

def test_while_loop_with_break_2():
    i = 0
    while i <= 5:
        i += 1

        if i % 3 == 0:
            break
        else:
            print i

## -----------------------------------------------------------------------------

def test_while_loop_nested_with_break():
    i = 0
    while i <= 5:
        i += 1

        j = 5
        while j > 0:
            j -= 1
            if j - 2 < 0:
                break
            else:
                print j

        if i % 3 == 0:
            break
        else:
            print i

## -----------------------------------------------------------------------------

def test_while_loop_nested_with_continue():
    i = 0
    while i <= 5:
        i += 1

        j = 5
        while j > 0:
            j -= 1
            if j - 2 < 0:
                continue
            else:
                print j

        if i % 3 == 0:
            continue
        else:
            print i

## -----------------------------------------------------------------------------

def test_while_loop_nested_with_break_and_continue():
    i = 0
    while i <= 5:
        i += 1

        j = 5
        while j > 0:
            j -= 2

            if j < 2:
                break

            k = 3
            while k > 0:
                k -= 1

                if (k + 3) % 2 == 0:
                    print k
                else:
                    continue

            print j

        print i

## -----------------------------------------------------------------------------

def test_while_else_loop():
    i = 0
    while i < 5:
        print i
        i += 1
    else:
        print i

## -----------------------------------------------------------------------------

def test_while_else_loop_with_break():
    i = 0
    while i < 5:
        print i
        i += 1
        if i > 3:
            break
    else:
        print i

## -----------------------------------------------------------------------------

def test_while_else_loop_with_continue():
    i = 0
    while i < 5:
        print i
        i += 1
        if i > 3:
            continue
    else:
        print i

## -----------------------------------------------------------------------------

def test_while_loop_with_exception():
    def inner():
        i = 0
        while i < 5:
            print i
            if i > 3:
                raise Exception()
            i += 1

    try:
        inner()
    except Exception:
        print 'Exception in while-else loop caught'

## -----------------------------------------------------------------------------

def test_while_else_loop_with_exception():
    def inner():
        i = 0
        while i < 5:
            print i
            if i > 3:
                raise Exception()
            i += 1
        else:
            print 'This never happens'

    try:
        inner()
    except Exception:
        print 'Exception in while-else loop caught again'

## -----------------------------------------------------------------------------

test_while_loop()
test_while_loop_with_continue()
test_while_loop_with_continue_2()
test_while_loop_with_break()
test_while_loop_with_break_2()
test_while_loop_nested_with_break()
test_while_loop_nested_with_continue()
test_while_loop_nested_with_break_and_continue()
test_while_else_loop()
test_while_else_loop_with_break()
test_while_else_loop_with_continue()
test_while_loop_with_exception()
test_while_else_loop_with_exception()

## -----------------------------------------------------------------------------
