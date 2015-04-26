## -----------------------------------------------------------------------------

def test_set_representation():
    print {3, 2, 1}

## -----------------------------------------------------------------------------

def test_set_add_and_clear():
    s  = set({4})
    print s
    s.add(3)
    print s
    s.add(2)
    print s
    s.add(1)
    print s
    s.add(3)
    print s
    s.clear()
    print s

## -----------------------------------------------------------------------------

def test_set_contains():
    s = {3, True, 'Hello world'}
    print 3 in s
    print True in s
    print 'Hello world' in s
    print 1 in s
    print 2 in s
    print 'Hi there' in s

## -----------------------------------------------------------------------------

def test_set_issubset_and_issuperset():
    s1 = {1, 2, 3}
    s2 = {1, 2, 3, 4}
    s3 = {1, 3, 5}

    print s1.issubset(s1)
    print s1.issubset(s2)
    print s1.issubset(s3)
    print s2.issubset(s1)
    print s3.issubset(s1)

    print s1.issuperset(s1)
    print s1.issuperset(s2)
    print s1.issuperset(s3)
    print s2.issuperset(s1)
    print s3.issuperset(s1)

## -----------------------------------------------------------------------------

def test_set_union():
    print set({4, 5, 6}).union(set({1, 2, 3}))

## -----------------------------------------------------------------------------

def test_set_remove():
    s = {3, 2, 1}
    s.remove(1)
    print s
    try:
        s.remove(4)
    except KeyError:
        print 'Cannot remove 4 from set'

    s.remove(3)
    print s

    s.remove(2)
    print s

    print s.__len__() == 0

## -----------------------------------------------------------------------------

def test_set_discard():
    s = {3, 2, 1}
    s.discard(3)
    print s

    s.discard(4)
    print s

    s.discard(1)
    print s

    s.discard('Hello world')
    print s

    s.discard(2)
    print s

    print s.__len__() == 0

## -----------------------------------------------------------------------------

def test_set_difference():
    s0 = set({})
    s1 = {1, 2, 3}
    s2 = {2, 3, 4}
    s3 = {3, 4, 5}

    print s0.difference(s0)

    print s1.difference(s0)
    print s1.difference(s1)
    print s1.difference(s2)
    print s1.difference(s3)

    print s0.difference(s1)
    print s1.difference(s1)
    print s2.difference(s1)
    print s3.difference(s1)

## -----------------------------------------------------------------------------

def test_set_update():
    s0 = set({})
    s1 = {6, 5, 4}
    s2 = {1, 2, 3}

    s0.update(s0)
    print s0

    s1.update(s1)
    print s1

    s1.update(s2)
    print s1

## -----------------------------------------------------------------------------

def test_set_difference_udpate():
    s0 = set({})
    s1 = {3, 2, 1}
    s2 = {4, 3, 2}
    s3 = {5, 4, 3}

    s0.difference_update(s0)
    print s0

    s0.difference_update(s1)
    print s0

    s1.difference_update(s0)
    print s1

    s1.difference_update(s1)
    print s1

    s2.difference_update(s3)
    print s2

    s3.difference_update(s1)
    print s3

## -----------------------------------------------------------------------------

def test_set_intersection():
    s0 = set({})
    s1 = {1, 2, 3}
    s2 = {2, 3, 4}
    s3 = {3, 4, 5}

    print s0.intersection(s0)

    print s1.intersection(s0)
    print s1.intersection(s1)
    print s1.intersection(s2)
    print s1.intersection(s3)

    print s0.intersection(s1)
    print s1.intersection(s1)
    print s2.intersection(s1)
    print s3.intersection(s1)

## -----------------------------------------------------------------------------

def test_set_intersection_udpate():
    s0 = set({})
    s1 = {3, 2, 1}
    s2 = {4, 3, 2}
    s3 = {5, 4, 3}

    s0.intersection_update(s0)
    print s0

    s1.intersection_update(s0)
    print s1

    s2.intersection_update(s3)
    print s2

    s3.intersection_update(s3)
    print s3

## -----------------------------------------------------------------------------

def test_set_symmetric_difference():
    setA = {"John", "Bob", "Mary", "Serena"}
    setB = {"Jim", "Mary", "John", "Bob"}

    print setA.symmetric_difference(setB)

    s1 = {3, 4, 5}
    s2 = {1, 2, 3}

    print s1.symmetric_difference(s2)

## -----------------------------------------------------------------------------

def test_set_symmetric_difference_update():
    setA = {"John", "Bob", "Mary", "Serena"}
    setB = {"Jim", "Mary", "John", "Bob"}

    setA.symmetric_difference_update(setB)
    print setA

    s0 = set({})
    s1 = {3, 4, 5}
    s2 = {1, 2, 3}

    s0.symmetric_difference_update(s0)
    print s0

    s1.symmetric_difference_update(s2)
    print s1

    s2.symmetric_difference_update(s0)
    print s2

## -----------------------------------------------------------------------------

def test_set_subtraction():
    s0 = set({})
    s1 = {1, 2, 3}
    s2 = {2, 3, 4}
    s3 = {3, 4, 5}

    print s0 - s0

    print s1 - s0
    print s1 - s1
    print s1 - s2
    print s1 - s3

    print s0 - s1
    print s1 - s1
    print s2 - s1
    print s3 - s1

## -----------------------------------------------------------------------------

def test_set_bitwise_or():
    print set({4, 5, 6}) | set({1, 2, 3})

## -----------------------------------------------------------------------------

def test_set_bitwise_and():
    s0 = set({})
    s1 = {1, 2, 3}
    s2 = {2, 3, 4}
    s3 = {3, 4, 5}

    print s0 & s0

    print s1 & s0
    print s1 & s1
    print s1 & s2
    print s1 & s3

    print s0 & s1
    print s1 & s1
    print s2 & s1
    print s3 & s1

## -----------------------------------------------------------------------------

def test_set_bitwise_xor():
    setA = {"John", "Bob", "Mary", "Serena"}
    setB = {"Jim", "Mary", "John", "Bob"}

    print setA ^ setB

    s1 = {3, 4, 5}
    s2 = {1, 2, 3}

    print s1 ^ s2

## -----------------------------------------------------------------------------

def test_set_equality():
    setA = {"John", "Bob", "Mary", "Serena"}
    setB = {"Jim", "Mary", "John", "Bob"}

    print setA == setA
    print setA != setA

    print setA == setB
    print setA != setB

    s1 = {3, 4, 5}
    s2 = {1, 2, 3}

    print s1 == s1
    print s1 != s1

    print s1 == s2
    print s1 != s2

## -----------------------------------------------------------------------------

test_set_representation()
test_set_add_and_clear()
test_set_contains()
test_set_issubset_and_issuperset()
test_set_union()
test_set_remove()
test_set_discard()
test_set_difference()
test_set_update()
test_set_difference_udpate()
test_set_intersection()
test_set_intersection_udpate()
test_set_symmetric_difference()
test_set_symmetric_difference_update()
test_set_subtraction()
test_set_bitwise_or()
test_set_bitwise_and()
test_set_bitwise_xor()
test_set_equality()

## -----------------------------------------------------------------------------
