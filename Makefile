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
all: src
	@find . -name "*.o" | xargs $(AR) $(ARFLAGS) $(LIBCOREVM)	
	@echo "\033[35mGenerated $(LIBCOREVM)"

.PHONY: src
src:
	@$(MAKE) -C $(SRC) all

.PHONY: test
test:
	@$(MAKE) -C $(TESTS) all
	@-for dir in $(TESTS); do (find $$dir -type f -name "*.test" -exec '{}' \;); done
	@echo "\033[32mTests run completed...\033[39m";

.PHONY: clean
clean:
	@-for dir in $(SUBDIRS); do ($(MAKE) -C $$dir clean;); done
	@rm -rf $(LIBCOREVM)
