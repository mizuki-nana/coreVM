## -----------------------------------------------------------------------------

def test_list_repr():
    print list([])
    print list([1])
    print list([1, 2])
    print list([1, 2, list([3,4,5])])
    print ['Hello', 'World!', ['Welcome', 'to', 'Python', [123, True, 9.654321]]]

## -----------------------------------------------------------------------------

def test_hash_list():
    try:
        [1, 2, 3].__hash__()
    except TypeError:
        print 'TypeError: unhashable type: list'

## -----------------------------------------------------------------------------

test_list_repr()
test_hash_list()

## -----------------------------------------------------------------------------
