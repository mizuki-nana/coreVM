def test_dict_repr_with_integer_keys():
    d = {
      123: 321,
      99: {
        8: ['Welcome', 'to', 'Python', {123: 123.654321}]
      }
    }
    print d
    print d.keys()
    print d.values()

    for key in d:
        print key

## -----------------------------------------------------------------------------

def test_dict_repr_with_str_keys():
    d = {
      'Hello': 'World!'
    }

    print d

## -----------------------------------------------------------------------------

def test_dict_repr_with_boolean_keys():
    d = {
      True: False
    }

    print d

## -----------------------------------------------------------------------------

def test_dict_repr_with_floating_number_keys():
    d = {
      1.123456: 99.987654
    }

    print d

## -----------------------------------------------------------------------------

def test_dict_hash():
    try:
        {1: 2}.__hash__()
    except TypeError:
        print 'TypeError: unhashable type: dict'

## -----------------------------------------------------------------------------

def test_dict_getitem():
    d = {
      1: 'Tomiko Van',
      2: 'Once upon a time...',
      'Hello': 'World',
    }
    print d[1]
    print d[2]
    print d['Hello']
    try:
        print d['some random key']
    except KeyError:
        print 'KeyError: key does not exist'

## -----------------------------------------------------------------------------

test_dict_repr_with_integer_keys()
test_dict_repr_with_str_keys()
test_dict_repr_with_boolean_keys()
test_dict_repr_with_floating_number_keys()
test_dict_hash()
test_dict_getitem()

## -----------------------------------------------------------------------------
