print True
print False
print bool(1)
print bool(0)
True is False
True ** False
False << True
True >> False
True | False
bool(0) | False
False & True
True & bool(1)
True ^ bool(0)
bool(0) ^ bool(1)
bool(1) // bool(1)

# Python returns int type.
print ~True
print ~bool(0)

print not True
print not bool(0)

print -True
print +False
print -(+True)
print +(-(+bool(0)))

# NOTE: Python prints arithmetic operations on bools as `1`s and `0`s, but we
# actually print them as `True` and `False`, so can't do stdout comparision.
# ( Kinda sad :'( )
# But the results were verified as correct.
# print True + False
# print True - False
# print True * False
# print False / True
# print False % True

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
