# The MIT License (MIT)

# Copyright (c) 2015 Yanzheng Li

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the 'Software'), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.


## -----------------------------------------------------------------------------

def test_chr_on_numbers_between_32_to_126():
    i = 32
    while i < 127:
        print chr(i)
        i += 1

## -----------------------------------------------------------------------------

def test_chr_on_special_cases():
    print chr(9)
    print chr(10)
    print chr(13)

## -----------------------------------------------------------------------------

def test_chr_on_remaining_valid_inputs():
    specials = set({9, 10, 13})

    # Need to handle escapinf characters.
    #
    # 39: "\'"
    # 92: "\\"
    cases_to_skip_separately = set({39, 92})
    inputs = list([])

    i = 0
    while i < 256:
      if not specials.__contains__(i) and not cases_to_skip_separately.__contains__(i):
          inputs.append(int(i))

      i += 1

    for i in inputs:
        print i, chr(i).__repr__()

## -----------------------------------------------------------------------------

def test_chr_on_invalid_inputs():
    def test_chr_with_valueerror(i):
        try:
            chr(i)
        except ValueError:
            print 'Expected ValueError for ' + i.__str__()

    test_chr_with_valueerror(-1)
    test_chr_with_valueerror(256)

## -----------------------------------------------------------------------------

test_chr_on_numbers_between_32_to_126()
test_chr_on_special_cases()
test_chr_on_remaining_valid_inputs()
test_chr_on_invalid_inputs()

## -----------------------------------------------------------------------------
