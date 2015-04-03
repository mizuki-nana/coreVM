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


func = lambda x: x * 2

print func(1)


def lambda_caller(arg):
    def inner(arg):
        func = lambda arg: arg * arg
        return func(arg)
    return inner(arg)


print lambda_caller(3)


print 'I can do math' if lambda_caller(1) else 'I suck at math'


print 'Nested if-esle WTF :D' if (True if 1 + 1 == 2 else False) else 'This is too confusing @__@'
