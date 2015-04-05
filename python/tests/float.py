## -------------------------- Representation Tests -----------------------------

print 5824.674626
print float(3.141592)

## -------------------------- Unary operator Tests -----------------------------

# NOTE: We cannot simply do `-9.999999` here because the Python `ast` module
# treats that as a single number instead of a unary negation operator applied
# on a floating number.
# (Python 2.7.6 on Ubuntu 14.04).
print -(9.999999)
print +10.123456
print +(-(-(123.725197)))

## ------------------------- Binary operator Tests -----------------------------

print 1.329431 + 5.953167
print 99.838301 - 99.000123
print 123.456 * 987.654
print 999.666333 / 3.00
# TODO: [COREVM-196] Modulus operator for float type inaccurate
#print 100.000001 % 33.000000
print 9.234 ** 4.76
print int(9.99 // 3.31) # printing precision issue

## --------------------------- Aug-assignment Tests ----------------------------

i = 1.34
i += 3.585739
print i

j = 111.555555
j -= 110.444444
print j

k = 123.554
k *= 321.649
print k

u = 777.777777
u /= 7
print u

v = 639.0
v %= 5.0
print v == float(639.0 % 5.0)

w = 23.444
w **= 3.59
print w

fd = 368.5
fd //= 5.0
print fd == int(368 // 5.0)


## ----------------------------- Comparison Tests ------------------------------

if 1.01 == 1.01:
    print 'A fact is a fact.'

if 1.01 != 1.02:
    print 'Zero tolerance on imprecision here'

if 1.0 < 1.1:
    print 'Precision is key'

if 1.123456789 <= 1.123456789:
    print 'Relentless pursuit of high-precision engineering'

if float(1.0) > float(0.99):
    print 'My statement still holds true'

if float(0.992) >= float(0.991):
    print 'You know the story'
