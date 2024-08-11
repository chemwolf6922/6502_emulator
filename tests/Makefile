CFALGS?=-g -O0

ALL_TESTS_SRC=$(wildcard *test.c)
ALL_TESTS=$(ALL_TESTS_SRC:.c=)

COMMON_SRC=test_framework.c ../e6502.c
COMMON_HEADERS=test_framework.h ../e6502.h ../asm6502.h

.PHONY: all
all: $(ALL_TESTS)

.PHONY: run
run: $(ALL_TESTS)
	@failed=0; \
	for test in $(ALL_TESTS); do \
		./$$test; \
		if [ $$? -ne 0 ]; then \
			failed=1; \
		fi; \
	done; \
	if [ $$failed -eq 1 ]; then \
		exit 1; \
	fi;

%test: %test.c $(COMMON_SRC) $(COMMON_HEADERS)
	$(CC) $(CFALGS) -o $@ $< $(COMMON_SRC)

run_%test: %test
	./$<

.PHONY: clean
clean:
	rm -f *test
