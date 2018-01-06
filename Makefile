SRC = ./src
TESTS = ./tests

TARGETS = $(SRC) $(TESTS)

.PHONY: all 
all: $(TARGETS)

.PHONY: src
$(SRC):
	@$(MAKE) -C $@;

.PHONY: tests
$(TESTS):
	@$(MAKE) -C $@;

.PHONY: clean
clean:
	$(MAKE) -C $(SRC) clean
	$(MAKE) -C $(TESTS) clean
