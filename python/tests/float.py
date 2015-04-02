print 5824.674626
print float(3.141592)
print 1.329431 + 5.953167
print 99.838301 - 99.000123
print 123.456 * 987.654
print 999.666333 / 3.00

# TODO: [COREVM-196] Modulus operator for float type inaccurate
#print 100.000001 % 33.000000

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
