i = 10

while i:
    print 'hello world'
    i = i - 1

## -----------------------------------------------------------------------------

i = 10
while i:
    break
    print 'This can never be said...'

## -----------------------------------------------------------------------------

i = 10
while i:
    i = i - 1
    continue
    print 'This can never be said...'
