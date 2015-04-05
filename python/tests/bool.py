## -------------------------- Representation Tests -----------------------------

print True
print False
print bool(1)
print bool(0)

## -------------------------- Unary operator Tests -----------------------------

# Python returns int type.
print ~True
print ~bool(0)

print not True
print not bool(0)

print -True
print +False
print -(+True)
print +(-(+bool(0)))

## ------------------------- Binary operator Tests -----------------------------

print True is False
print True is not False

# NOTE: Python prints arithmetic operations on bools as `1`s and `0`s, but we
# actually print them as `True` and `False`.
print (True + False) == 1
print (True - False) == 1
print (True * False) == 0
print (False / True) == 0
print (False % True) == 0
print (True ** False) == 1
print (False << True) == 1
print (True >> False) == 1
print (True | False) == 1
print (bool(0) | False) == 1
print (False & True) == 0
print (True & bool(1)) == 1
print (True ^ bool(0)) == 1
print (bool(0) ^ bool(1)) == 1
print (bool(1) // bool(1)) == 0

## --------------------------- Aug-assignment Tests ----------------------------

# NOTE: In Python (tested in v2.7.6), aug-assignments on instances of `bool`
# type makes the instance itself turn into type `int`. This behavior, however,
# it not currently supported.
i = True
i += False
print i == (True + False)

j = True
j -= False
print j == (True - False)

k = False
k *= bool(1)
print k == (False * bool(1))

u = bool(1)
u /= bool(1)
print u == (bool(1) / bool(1))

v = bool(1)
v %= True
print v == (bool(1) % True)

w = bool(1)
w **= bool(0)
print w == (bool(1) ** bool(0))

x = True
x <<= True
print x == (True << True)

y = False
y >>= True
print y == (False >> True)

z = True
z |= False
print z == (True | False)

zeta = bool(1)
zeta ^= bool(0)
print zeta == (bool(1) ^ bool(0))

zelda = bool(0)
zelda &= bool(1)
print zelda == (bool(0) & bool(1))

fd = True
fd //= bool(1)
print fd == (True // bool(1))

## ----------------------------- Comparison Tests ------------------------------

if True == bool(1):
    print 'Integrity of truth is rock solid'

if False != True:
    print 'Opposite polarity do not match'

if False < True:
    print 'The truth always triumphs'

if bool(0) <= bool(1):
    print 'The truth still triumphs'

if True > False:
    print 'The truth remains triumphant'

if bool(100) >= bool(0):
    print 'The truth remains triumphant eternally'
