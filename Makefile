# The MIT License (MIT)

# Copyright (c) 2014 Yanzheng Li

# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

AR=ar
ARFLAGS=rvs

INCLUDE=./include
SRC=./src
TESTS=./tests

SUBDIRS=$(SRC) $(TESTS)

LIBCOREVM=libcorevm.a

### Environment variables ###
export GTEST_COLOR=true


.PHONY: all
all:
	@$(MAKE) -C $(SRC)
	@find . -name "*.o" | xargs $(AR) $(ARFLAGS) $(LIBCOREVM)
	@echo "\033[35mGenerated $(LIBCOREVM)"

.PHONY: test
test:
	@$(MAKE) -C $(TESTS)
	@-for dir in $(TESTS); do (find $$dir -type f -name "*.test" -exec '{}' \;); done
	@echo "\033[32mTests run completed...\033[39m";

.PHONY: clean
clean:
	@-for dir in $(SUBDIRS); do ($(MAKE) -C $$dir clean;); done
	@-rm -rf $(LIBCOREVM)
