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
