AR=ar
ARFLAGS=rvs

INCLUDE=./include
SRC=./src
TESTS=./tests

SUBDIRS=$(SRC) $(TESTS)

LIBCOREVM=libcorevm.a

.PHONY: src
src:
	@$(MAKE) -C $(SRC) all

.PHONY: all
all: src
	@find . -name "*.o" | xargs $(AR) $(ARFLAGS) $(LIBCOREVM)	

.PHONY: test
test:
	@$(MAKE) -C $(TESTS) all

.PHONY: clean
clean:
	@-for dir in $(SUBDIRS); do ($(MAKE) -C $$dir clean;); done
