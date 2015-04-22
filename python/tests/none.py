## -----------------------------------------------------------------------------

def test_none_type_representation():
    print None

    a = None
    print a

    b = not None
    print b

    c = not not None
    print c
## -----------------------------------------------------------------------------

def test_none_type_logic():
    if None:
        print "This shouldn't be said"

    if not None:
        print 'This instead, can be said'

## -----------------------------------------------------------------------------

def test_none_type_hash():
    print None.__hash__() > 0

## -----------------------------------------------------------------------------

test_none_type_representation()
test_none_type_logic()
test_none_type_hash()

## -----------------------------------------------------------------------------
