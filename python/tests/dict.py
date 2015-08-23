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

def test_dict_setitem():
    d = {
      1: 1,
      2: 20,
      3: 300,
      'zigzagoon': True
    }
    d[1] = 10
    print d[1]
    d[2] *= 2
    print d[2]
    d[3] -= 100
    print d[3]
    d['zigzagoon'] = False
    print d['zigzagoon']

    d['Hello'] = 'world'
    print d['Hello']

    try:
        d[4] += 1
    except KeyError:
        print 'KeyError: key does not exist'

## -----------------------------------------------------------------------------

def test_dict_comprehension():
    d = {
      1: 1,
      2: 2,
      3: 3,
    }

    c = 2

    print {k: v for k, v in d.items()}

    print {k: v ** c for k, v in d.items()}

    print {k: v ** c for k, v in d.items() if k % 2 == 0}

    print {k: k * c for k in d.keys()}

    print {v * c: v for v in d.values()}

    print {v * c: v for v in d.values() if v % 2 == 0}

## -----------------------------------------------------------------------------

test_dict_repr_with_integer_keys()
test_dict_repr_with_str_keys()
test_dict_repr_with_boolean_keys()
test_dict_repr_with_floating_number_keys()
test_dict_hash()
test_dict_getitem()
test_dict_setitem()
test_dict_comprehension()

## -----------------------------------------------------------------------------
