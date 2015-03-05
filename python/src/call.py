def do_math(*args):
  1 + 1

def hello_world(arg1, arg2, kwarg1=1+1, *args, **kwargs):
  do_math(arg1)

hello_world(1, 2, kwarg1=3)
