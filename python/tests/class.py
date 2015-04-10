class MyObject(object):

    def __init__(self, arg):
        pass

    def run(self, arg):
        print arg


o = MyObject(1)
o.run('Hello world')
o.name = 'Will'
print o.name
