CFLAGS?=-O3
override CFLAGS+=-MP -MMD
LDFLAGS?=

APP=basic-vm
SRC=basic-vm.c ../e6502.c

.PHONY:all
all: $(APP)

$(APP): $(patsubst %.c,%.o,$(SRC))
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

-include $(SRC:.c=.d)

.PHONY:clean
clean:
	rm -f $(APP) *.o *.d
