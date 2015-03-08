def hello_world_again(*args):
  print 'Hello world again!'

def introduction():
  print 'My name is Will :-)'
  # test explicit return should work.
  return

def hello_world(*args):
  print 'Hello world!'
  introduction()

def main(arg1, arg2, *args, **kwargs):
  print 'Hi'
  hello_world(arg1)
  hello_world_again()
  hello_world(arg2)
  print 'Bye'

main(1, 2)
print 'Done!'
