LLVM_PORT_DIR=../llvm-mos-port
LLVM_PORT_SRC=$(LLVM_PORT_DIR)/port.c

CC=mos-common-clang
CFLAGS?=-Os -flto
LDFLAGS?=
override LDFLAGS+=-lexit-custom
override LDFLAGS+=-linit-stack
override LDFLAGS+=-Wl,-T,$(LLVM_PORT_DIR)/link.ld

.PHONY: all
all: main

main: main.c $(LLVM_PORT_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	mv $@ $@.bin

.PHONY: dump
dump: main.elf
	llvm-objdump -d --print-imm-hex $<

.PHONY: clean
clean:
	rm -f *.bin *.elf
