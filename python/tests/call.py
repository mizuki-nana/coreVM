def do_math(arg):
    print 'Time to have fun!'
    return int(5)

def hello_world_again(arg):
    print 'Hello world again!'

def introduction(arg):
    print 'My name is Will :-)'
    # test explicit return should work.
    return

def hello_world(arg):
    print 'Hello world!'
    introduction(arg)

def main(arg):
    print 'Hi'
    hello_world(arg)
    hello_world_again(do_math(arg))
    hello_world(arg)
    print 'Bye'

print 'Starting...'
main(int(1))
print 'Done!'
