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

class AnotherException(Exception):

    def __init__(self):
        pass

## -----------------------------------------------------------------------------

class YetAnotherException(Exception):

    def __init__(self):
        pass

    def __str__(self):
        return 'Yet another exception'

## -----------------------------------------------------------------------------

def test_simple_try_except():
    try:
        raise Exception()
    except Exception as exc:
        print 'Everything is going to be okay'

## -----------------------------------------------------------------------------

def test_multiple_except_blocks():
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

## -----------------------------------------------------------------------------

def test_catch_exception_from_function_call():
    try:
        hello_world()
    except Exception:
        print 'Catching exception from another function'

## -----------------------------------------------------------------------------

def greetings():
    hello_world()

## -----------------------------------------------------------------------------

def test_catch_exception_from_function_call_2():
    try:
        greetings()
    except Exception:
        print 'Catching exception from nested function'

## -----------------------------------------------------------------------------

def welcome():
    greetings()

## -----------------------------------------------------------------------------

def test_catch_exception_from_function_call_3():
    try:
        welcome()
    except Exception:
        print 'Catching exception through triple calls'

## -----------------------------------------------------------------------------

def catch_exception_from_same_level_of_except_block():
    try:
        raise AnotherException()
    except AnotherException:
        raise YetAnotherException()
    except YetAnotherException:
        print 'Catching exception from same level of except blocks is not supported in Python'

## -----------------------------------------------------------------------------

def test_catch_exception_from_same_level_of_except_block():
    try:
        catch_exception_from_same_level_of_except_block()
    except YetAnotherException:
        print 'Catching exception from exception raised in except block'

## -----------------------------------------------------------------------------

def explode():
    raise Exception()

## -----------------------------------------------------------------------------

def handle_explosion():
    try:
        explode()
    except Exception:
        raise AnotherException()

## -----------------------------------------------------------------------------

def do_something():
    try:
        handle_explosion()
    except AnotherException:
        raise YetAnotherException()

## -----------------------------------------------------------------------------

def test_complex_try_except():
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

## -----------------------------------------------------------------------------

def test_complex_try_except_2():
    try:
        handle_explosion2()
    except Exception:
        print 'Explosion handled again'

## -----------------------------------------------------------------------------

def parent():
    def inner2():
        raise Exception()

    return inner2

## -----------------------------------------------------------------------------

def test_exception_from_inner_function():
    try:
        parent()()
    except Exception:
        print 'Exception from nested function caught.'

## -----------------------------------------------------------------------------

# TODO: [COREVM-275] Python compiler elide closure of previously-named function
def run2():
    parent()()

## -----------------------------------------------------------------------------

def test_exception_from_inner_function_2():
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

def test_try_except_else_with_no_exception():
    # Tests that statements under the 'else' block get executed when no
    # exceptions are raised.

    try:
        print 'Hi'
    except Exception:
        print 'Error'
    else:
        print 'Bye'

## -----------------------------------------------------------------------------

def test_try_except_else_with_exception():
    # Tests that an exception gets raised in the 'try' block can be
    # caught in the associated 'except' block, and that statements under the
    # 'else' block are skipped.

    try:
        raise Exception()
    except Exception:
        print 'Error'
    else:
        print 'Bye'

## -----------------------------------------------------------------------------

def test_try_except_finally_with_no_exception():
    # Tests that statements under the 'finally' block get executed when no
    # exception are thrown.

    try:
        print 'Hi'
    except Exception:
        print 'Go away!'
    else:
        print 'Hello'
    finally:
        print 'Bye'

## -----------------------------------------------------------------------------

def test_try_except_finally_with_caught_exception():
    # Tests that statements under the 'finally' block get executed when an
    # exception gets caught by the 'except' block.

    try:
        raise AnotherException()
    except AnotherException:
        print 'Caught exception'
    else:
        print 'This should not happen'
    finally:
        print 'Bye'

## -----------------------------------------------------------------------------

def test_try_except_finally_with_uncaught_exception():
    # Tests that an uncaught exception raised in a function
    # can be caught by the 'except' block on the outer level that has a
    # 'finally' block.

    def inner():
        try:
            raise YetAnotherException()
        except AnotherException:
            print 'Should have not caught exception'
        finally:
            print 'Having an error that cannot handle here'

    try:
        inner()
    except YetAnotherException:
        print 'Caught inner level error'
    finally:
        print 'Done work. Doing clean up'

## -----------------------------------------------------------------------------

def test_try_except_finally_with_uncaught_inner_exception():
    # Tests that an exception raised in a nested function can be propagated
    # through two levels of 'finally' blocks on the outer levels.

    def inner2():
        try:
            raise TypeError()
        except AnotherException:
            print 'Should have not caught exception'
        finally:
            print 'Having an error that cannot handle here'

    def inner():
        try:
            inner2()
        except YetAnotherException:
            print 'Only accept more granular errors here'
        finally:
            print 'Let it go'

    try:
        inner()
    except TypeError:
        print 'Accept type errors here'
    finally:
        print 'Done. Happy Friday'

## -----------------------------------------------------------------------------

def test_try_except_finally_with_exception_from_inner_finally():
    # Tests that an exception raised in a nested 'finally' block can be
    # caught by an 'except' block in the outer level.

    try:
        try:
            raise YetAnotherException()
        except TypeError:
            print 'Accepting type error'
        finally:
            raise AnotherException()
    except AnotherException:
        print 'Accepting another exception here'
    else:
        print 'EMERGENCY ONLY'
    finally:
        print 'We are good'

## -----------------------------------------------------------------------------

def test_try_except_finally_with_exception_from_inner_except():
    # Tests that an exception raised in a nested 'except' block can be caught
    # in the associated 'finally' block and propagated to the outer level.

    try:
        try:
            raise AnotherException()
        except AnotherException:
            print 'Accepting another error'
            raise YetAnotherException()
        finally:
            print 'Something could be wrong here'
    except YetAnotherException:
        print 'Accepting yet another exception here'
    else:
        print 'EMERGENCY ONLY'
    finally:
        print 'We are good'

## -----------------------------------------------------------------------------

def test_try_except_finally_with_nested_exceptions_raised_at_different_places():
    # Tests that an exception raised in a nested 'except' block can be
    # caught through the outer 'finally' block.

    try:
        try:
            try:
                raise Exception()
            except Exception:
                raise AnotherException()
        except TypeError:
            print 'Only accept TypeError here'
        finally:
            print 'Things could go wrong here'
            raise YetAnotherException()
    except YetAnotherException:
        print 'Handling yet another exception'
    finally:
        print 'Handled yet another exception'

## -----------------------------------------------------------------------------

def test_try_except_finally_with_nested_exceptions_raised_at_different_places_2():
    # Tests that an exception raised in a nested 'finally' block can be
    # caught by the outer 'except' block.

    try:
        try:
            try:
                raise Exception()
            except Exception:
                raise AnotherException()
            else:
                print 'Hello'
            finally:
                print 'Finally'
        except AnotherException:
            print 'Accepting another exception here'
        finally:
            raise YetAnotherException()
    except YetAnotherException:
        print 'Accepting yet another exception here'
    else:
        print 'Bonjour'
    finally:
        print 'Ja ma ta'

## -----------------------------------------------------------------------------

def test_adjacent_try_except_blocks():
    # Tests that adjacent 'try-except' blocks do not interfere with each other.

    try:
        raise TypeError()
    except TypeError:
        print 'TypeError handled'
    finally:
        print 'Everyone is happy'

    try:
        print 'Nothing happens here'
    except Exception:
        print 'We dont expect anything to happen'
    else:
        print 'Cuz its Boring Town'

## -----------------------------------------------------------------------------

def test_try_except_in_locally_defined_function():
    # Tests that an exception raised in a nested function that
    # has no 'finally' block can be caught in the outer try-except block.

    try:
        def inner():
            try:
                raise AnotherException()
            except AnotherException:
                raise YetAnotherException()

        inner()
    except YetAnotherException:
        print 'Accepting yet another exception'
    else:
        print 'Nothing to do here'

## -----------------------------------------------------------------------------

def test_try_except_in_locally_defined_function_2():
    # Tests that an exception raised in a nested function that
    # has a 'finally' block can be caught in the outer try-except block.

    try:
        def inner():
            try:
                raise AnotherException()
            except AnotherException:
                raise YetAnotherException()
            finally:
                print 'Clean up'

        inner()
    except YetAnotherException:
        print 'Accepting yet another exception'
    else:
        print 'Nothing to do here'
    finally:
        print 'More clean up'

## -----------------------------------------------------------------------------

def test_try_except_in_locally_defined_function_3():
    # Tests that an exception raised in a nested function that
    # has no 'finally' block, but has a neighboring 'try-except-finally' block,
    # can be caught in the outer try-except block.

    try:
        def inner():
            try:
                print 'Hello'
            except Exception:
                print 'This shouldn not happen'
            finally:
                print 'This is a peaceful world'

            try:
                raise AnotherException()
            except AnotherException:
                raise YetAnotherException()

        inner()
    except YetAnotherException:
        print 'Accepting yet another exception'
    else:
        print 'Nothing to do here'

## -----------------------------------------------------------------------------

def test_break_in_try_finally_block_under_for_loop():
    ls = [1, 2, 3, 4, 5]
    for i in ls:
        try:
            if i == 3:
                break
            print i
        except Exception:
            pass
        finally:
            print 'Ha', i

## -----------------------------------------------------------------------------

def test_continue_in_try_finally_block_under_for_loop():
    ls = [1, 2, 3, 4, 5]
    for i in ls:
        try:
            if i == 3:
                continue
            print i
        except Exception:
            pass
        finally:
            print 'Ha', i

## -----------------------------------------------------------------------------

def test_return_in_try_finally_block_under_for_loop():
    ls = [1, 2, 3, 4, 5]
    for i in ls:
        try:
            if i == 3:
                return
            print i
        except Exception:
            pass
        finally:
            print 'Ha', i

## -----------------------------------------------------------------------------

def test_break_in_try_finally_block_under_for_loop_and_another_try_block():
    ls = [1, 2, 3, 4, 5]
    for i in ls:
        try:
            if i == 3:
                try:
                    break
                except Exception:
                    pass
            print i
        except Exception:
            pass
        finally:
            print 'Ha', i

## -----------------------------------------------------------------------------

def test_continue_in_try_finally_block_under_for_loop_and_another_try_block():
    ls = [1, 2, 3, 4, 5]
    for i in ls:
        try:
            if i == 3:
                try:
                    continue
                except Exception:
                    pass
            print i
        except Exception:
            pass
        finally:
            print 'Ha', i

## -----------------------------------------------------------------------------

def test_return_in_try_finally_block_under_for_loop_and_another_try_block():
    ls = [1, 2, 3, 4, 5]
    for i in ls:
        try:
            if i == 3:
                try:
                    return
                except Exception:
                    pass
            print i
        except Exception:
            pass
        finally:
            print 'Ha', i

## -----------------------------------------------------------------------------

def test_break_in_try_finally_block_under_for_loop_and_another_try_finally_block():
    ls = [1, 2, 3, 4, 5]
    for i in ls:
        try:
            if i == 3:
                try:
                    break
                except Exception:
                    pass
                finally:
                    print 'Hello'
            print i
        except Exception:
            pass
        finally:
            print 'Ha', i

## -----------------------------------------------------------------------------

def test_continue_in_try_finally_block_under_for_loop_and_another_try_finally_block():
    ls = [1, 2, 3, 4, 5]
    for i in ls:
        try:
            if i == 3:
                try:
                    continue
                except Exception:
                    pass
                finally:
                    print 'Hello'
            print i
        except Exception:
            pass
        finally:
            print 'Ha', i

## -----------------------------------------------------------------------------

def test_return_in_try_finally_block_under_for_loop_and_another_try_finally_block():
    ls = [1, 2, 3, 4, 5]
    for i in ls:
        try:
            if i == 3:
                try:
                    return
                except Exception:
                    pass
                finally:
                    print 'Hello'
            print i
        except Exception:
            pass
        finally:
            print 'Ha', i

## -----------------------------------------------------------------------------

def test_break_in_try_finally_block_under_nested_for_loop():
    ls = [1, 2, 3, 4, 5]
    for i in ls:
        for j in ls:
            try:
                if j == 3:
                    break
                print j
            except Exception:
                pass
            finally:
                print 'Ha', j
        print i

## -----------------------------------------------------------------------------

def test_continue_in_try_finally_block_under_nested_for_loop():
    ls = [1, 2, 3, 4, 5]
    for i in ls:
        for j in ls:
            try:
                if j == 3:
                    continue
                print j
            except Exception:
                pass
            finally:
                print 'Ha', j
        print i

## -----------------------------------------------------------------------------

def test_return_in_try_finally_block_under_nested_for_loop():
    ls = [1, 2, 3, 4, 5]
    for i in ls:
        for j in ls:
            try:
                if j == 3:
                    return
                print j
            except Exception:
                pass
            finally:
                print 'Ha', j
        print i

## -----------------------------------------------------------------------------

def test_break_in_nested_try_finally_blocks_and_for_loops():
    ls = [1, 2, 3, 4, 5]
    for i in ls:
        try:
            for j in ls:
                try:
                    if j == 3:
                        break
                    print j
                except Exception:
                    pass
                finally:
                    print 'Ja', j

            if i == 3:
                break

            print i
        except Exception:
            pass
        finally:
            print 'Ha', i

## -----------------------------------------------------------------------------

def test_continue_in_nested_try_finally_blocks_and_for_loops():
    ls = [1, 2, 3, 4, 5]
    for i in ls:
        try:
            for j in ls:
                try:
                    if j == 3:
                        continue
                    print j
                except Exception:
                    pass
                finally:
                    print 'Ja', j

            if i == 3:
                break

            print i
        except Exception:
            pass
        finally:
            print 'Ha', i

## -----------------------------------------------------------------------------

def test_return_in_nested_try_finally_blocks_and_for_loops():
    ls = [1, 2, 3, 4, 5]
    for i in ls:
        try:
            for j in ls:
                try:
                    if j == 3:
                        return
                    print j
                except Exception:
                    pass
                finally:
                    print 'Ja', j

            if i == 3:
                break

            print i
        except Exception:
            pass
        finally:
            print 'Ha', i

## -----------------------------------------------------------------------------

def test_return_in_triple_nested_finally_blocks_and_for_loops():
    ls = [1, 2, 3, 4, 5]
    for i in ls:
        try:
            for j in ls:
                try:
                    for k in ls:
                        try:
                            if k % 2 == 0:
                                return
                        except Exception:
                            pass
                        finally:
                            print 'Ka', k

                    if j == 4:
                        break

                    print j
                except Exception:
                    pass
                finally:
                    print 'Ja', j

            if i == 3:
                break

            print i
        except Exception:
            pass
        finally:
            print 'Ha', i

## -----------------------------------------------------------------------------

def test_return_in_triple_nested_finally_blocks_and_for_loops_2():
    ls = [1, 2, 3, 4, 5]
    for i in ls:
        try:
            for j in ls:
                try:
                    for k in ls:
                        try:
                            if k % 2 == 0:
                                return
                        except Exception:
                            pass
                        finally:
                            print 'Ka', k

                    if j == 4:
                        return

                    print j
                except Exception:
                    pass
                finally:
                    print 'Ja', j

            if i == 3:
                return

            print i
        except Exception:
            pass
        finally:
            print 'Ha', i

## -----------------------------------------------------------------------------

def test_break_in_try_finally_block_in_nested_for_loops_with_outer_branch():
    ls = [1, 2, 3, 4, 5]
    for i in ls:
        for j in ls:
            try:
                if j == 3:
                    break
                print j
            except Exception:
                pass
            finally:
                print 'Ja', j

        if i == 3:
            break

        print i

## -----------------------------------------------------------------------------

def test_continue_in_try_finally_block_in_nested_for_loops_with_outer_branch():
    ls = [1, 2, 3, 4, 5]
    for i in ls:
        for j in ls:
            try:
                if j == 3:
                    continue
                print j
            except Exception:
                pass
            finally:
                print 'Ja', j

        if i == 3:
            break

        print i

## -----------------------------------------------------------------------------

def test_return_in_try_finally_block_in_nested_for_loops_with_outer_branch():
    ls = [1, 2, 3, 4, 5]
    for i in ls:
        for j in ls:
            try:
                if j == 3:
                    return
                print j
            except Exception:
                pass
            finally:
                print 'Ja', j

        if i == 3:
            break

        print i

## -----------------------------------------------------------------------------

test_simple_try_except()
test_multiple_except_blocks()
test_catch_exception_from_function_call()
test_catch_exception_from_function_call_2()
test_catch_exception_from_function_call_3()
test_catch_exception_from_same_level_of_except_block()
test_complex_try_except()
test_complex_try_except_2()
test_exception_from_inner_function()
test_exception_from_inner_function_2()

test_try_except_else_with_exception()
test_try_except_else_with_no_exception()
test_try_except_finally_with_no_exception()
test_try_except_finally_with_caught_exception()
test_try_except_finally_with_uncaught_exception()
test_try_except_finally_with_uncaught_inner_exception()
test_try_except_finally_with_exception_from_inner_finally()
test_try_except_finally_with_exception_from_inner_except()
test_try_except_finally_with_nested_exceptions_raised_at_different_places()
test_try_except_finally_with_nested_exceptions_raised_at_different_places_2()
test_adjacent_try_except_blocks()
test_try_except_in_locally_defined_function()
test_try_except_in_locally_defined_function_2()
test_try_except_in_locally_defined_function_3()

test_break_in_try_finally_block_under_for_loop()
test_continue_in_try_finally_block_under_for_loop()
test_return_in_try_finally_block_under_for_loop()
test_break_in_try_finally_block_under_for_loop_and_another_try_block()
test_continue_in_try_finally_block_under_for_loop_and_another_try_block()
test_return_in_try_finally_block_under_for_loop_and_another_try_block()
test_break_in_try_finally_block_under_for_loop_and_another_try_finally_block()
test_continue_in_try_finally_block_under_for_loop_and_another_try_finally_block()
test_return_in_try_finally_block_under_for_loop_and_another_try_finally_block()

test_break_in_try_finally_block_under_nested_for_loop()
test_continue_in_try_finally_block_under_nested_for_loop()
test_return_in_try_finally_block_under_nested_for_loop()

test_break_in_nested_try_finally_blocks_and_for_loops()
test_continue_in_nested_try_finally_blocks_and_for_loops()
test_return_in_nested_try_finally_blocks_and_for_loops()

test_return_in_triple_nested_finally_blocks_and_for_loops()
test_return_in_triple_nested_finally_blocks_and_for_loops_2()

test_break_in_try_finally_block_in_nested_for_loops_with_outer_branch()
test_continue_in_try_finally_block_in_nested_for_loops_with_outer_branch()
test_return_in_try_finally_block_in_nested_for_loops_with_outer_branch()

## -----------------------------------------------------------------------------
