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

def test_for_loop_iterate_list():
    for i in [1, 2, 3, 4, 5]:
        print 'Hello world'
        print i

## -----------------------------------------------------------------------------

def test_for_loop_iterate_list_and_break_once():
    for i in [1, 2, 3, 4, 5]:
        print i
        break

## -----------------------------------------------------------------------------

def test_for_loop_iterate_list_and_break_immediately():
    for i in [1, 2, 3, 4, 5]:
        break
        print 'This can never be said...'

## -----------------------------------------------------------------------------

def test_for_loop_iterate_list_and_continue():
    for i in [5, 4, 3, 2, 1]:
        print i
        continue

## -----------------------------------------------------------------------------

def test_for_loop_iterate_list_and_continue_immediately():
    for i in [1, 2, 3, 4, 5]:
        continue
        print 'This can never be said...'

## -----------------------------------------------------------------------------

def test_for_loop_iterate_list_and_break_on_condition():
    for i in [1, 2, 3, 4, 5]:
        if i % 2 == 0:
            print i
        else:
            break

## -----------------------------------------------------------------------------

def test_nested_for_loops_with_mixed_continue_and_break():
    for i in [1, 2, 3, 4, 5]:
        if i % 2 == 0:
            print i
        else:
            continue

        for j in [5, 4, 3, 2, 1]:
            if j > 1:
                break
            else:
                print j

## -----------------------------------------------------------------------------

def test_triple_nested_for_loops_with_mixed_continues_and_breaks():
    for i in [1, 2, 3]:
        if i == 2:
            continue

        for j in [4, 5, 6]:
            if j == 6:
                break

            for k in [7, 8, 9]:
                if k % 3 == 0:
                    break
                else:
                    print k

            print j

        print i

## -----------------------------------------------------------------------------

def test_nested_for_loops_iterate_lists():
    for i in [1, 2, 3, 4, 5]:
        for j in [1, 2, 3, 4, 5]:
            print i * j

## -----------------------------------------------------------------------------

def test_nested_for_loops_iterate_lists_2():
    for i in [5, 4, 3, 2, 1]:
        for j in [1, 2, 3, 4, 5]:
            if i == j:
                print i

## -----------------------------------------------------------------------------

test_for_loop_iterate_list()
test_for_loop_iterate_list_and_break_once()
test_for_loop_iterate_list_and_break_immediately()
test_for_loop_iterate_list_and_continue()
test_for_loop_iterate_list_and_continue_immediately()
test_for_loop_iterate_list_and_break_on_condition()
test_nested_for_loops_with_mixed_continue_and_break()
test_triple_nested_for_loops_with_mixed_continues_and_breaks()
test_nested_for_loops_iterate_lists()
test_nested_for_loops_iterate_lists_2()

## -----------------------------------------------------------------------------
