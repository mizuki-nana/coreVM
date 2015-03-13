def hello_world_again():
    print 'Hello world again!'

def introduction():
    print 'My name is Will :-)'
    # test explicit return should work.
    return

def hello_world():
    print 'Hello world!'
    introduction()

def main():
    print 'Hi'
    hello_world()
    hello_world_again()
    hello_world()
    print 'Bye'

main()
print 'Done!'
