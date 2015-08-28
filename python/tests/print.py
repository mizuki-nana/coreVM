# The MIT License (MIT)

# Copyright (c) 2015 Yanzheng Li

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.


## -----------------------------------------------------------------------------

def test_print():
    print 0
    print 1, 2, 3
    print 'Hello', 'World', '!!!'
    print
    print 'Bye', 'Sayonara'
    print

## -----------------------------------------------------------------------------

def test_print_escape_characters():
    print '\n'
    print "Don\'t do that"
    print "She said \"hi\""
    print "She said \'hi\'"
    print "Who are you\?"
    print "\\Backslash"
    print "1\r2"
    print "12\b3"

## -----------------------------------------------------------------------------

test_print()
test_print_escape_characters()

## -----------------------------------------------------------------------------
