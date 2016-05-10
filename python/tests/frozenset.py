# The MIT License (MIT)

# Copyright (c) 2016 Yanzheng Li

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

def test_frozenset_representation():
    print frozenset(set({"Hello"}))

## -----------------------------------------------------------------------------

def test_frozenset_contains():
    s = frozenset(set({3, True, 'Hello world'}))
    print 3 in s
    print True in s
    print 'Hello world' in s
    print 1 in s
    print 2 in s
    print 'Hi there' in s

## -----------------------------------------------------------------------------

def test_frozenset_issubset_and_issuperset():
    s1 = frozenset(set({1, 2, 3}))
    s2 = frozenset(set({1, 2, 3, 4}))
    s3 = frozenset(set({1, 3, 5}))

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

def test_frozenset_union():
    print frozenset(set({4, 5, 6})).union(frozenset(set({1, 2, 3})))

## -----------------------------------------------------------------------------

def test_frozenset_difference():
    s0 = frozenset(set({}))
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

def test_frozenset_intersection():
    s0 = frozenset(set({}))
    s1 = frozenset(set({1, 2, 3}))
    s2 = frozenset(set({2, 3, 4}))
    s3 = frozenset(set({3, 4, 5}))

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

def test_frozenset_symmetric_difference():
    setA = frozenset(set({"John", "Bob", "Mary", "Serena"}))
    setB = frozenset(set({"Jim", "Mary", "John", "Bob"}))

    print setA.symmetric_difference(setB)

    s1 = {3, 4, 5}
    s2 = {1, 2, 3}

    print s1.symmetric_difference(s2)

## -----------------------------------------------------------------------------

def test_frozenset_bitwise_or():
    print frozenset(set({4, 5, 6})) | frozenset(set({1, 2, 3}))

## -----------------------------------------------------------------------------

def test_frozenset_bitwise_and():
    s0 = frozenset(set({}))
    s1 = frozenset(set({1, 2, 3}))
    s2 = frozenset(set({2, 3, 4}))
    s3 = frozenset(set({3, 4, 5}))

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

def test_frozenset_bitwise_xor():
    setA = frozenset(set({"John", "Bob", "Mary", "Serena"}))
    setB = frozenset(set({"Jim", "Mary", "John", "Bob"}))

    print setA ^ setB

    s1 = frozenset(set({3, 4, 5}))
    s2 = frozenset(set({1, 2, 3}))

    print s1 ^ s2

## -----------------------------------------------------------------------------

def test_frozenset_equality():
    setA = frozenset(set({"John", "Bob", "Mary", "Serena"}))
    setB = frozenset(set({"Jim", "Mary", "John", "Bob"}))

    print setA == setA
    print setA != setA

    print setA == setB
    print setA != setB

    s1 = frozenset(set({3, 4, 5}))
    s2 = frozenset(set({1, 2, 3}))

    print s1 == s1
    print s1 != s1

    print s1 == s2
    print s1 != s2

## -----------------------------------------------------------------------------

test_frozenset_representation()
test_frozenset_contains()
test_frozenset_issubset_and_issuperset()
test_frozenset_union()
test_frozenset_intersection()
test_frozenset_symmetric_difference()
test_frozenset_bitwise_or()
test_frozenset_bitwise_and()
test_frozenset_bitwise_xor()
test_frozenset_equality()

## -----------------------------------------------------------------------------
