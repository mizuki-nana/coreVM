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
