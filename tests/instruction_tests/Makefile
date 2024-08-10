CFALGS?=-g -O0

ALL_TESTS_SRC=$(wildcard *test.c)
ALL_TESTS=$(ALL_TESTS_SRC:.c=)

COMMON_SRC=test_framework.c ../../e6502.c
COMMON_HEADERS=test_framework.h ../../e6502.h ../../asm6502.h

.PHONY: all
all: $(ALL_TESTS)

.PHONY: run
run: $(ALL_TESTS)
	@for test in $(ALL_TESTS); do \
		echo "--- Running $$test ---"; \
		./$$test; \
	done

%test: %test.c $(COMMON_SRC) $(COMMON_HEADERS)
	$(CC) $(CFALGS) -o $@ $< $(COMMON_SRC)

.PHONY: clean
clean:
	rm -f $(ALL_TESTS)
