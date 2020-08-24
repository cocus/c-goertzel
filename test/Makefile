
C_GOERTZEL_PATH=../src

CC ?= gcc
AR ?= ar
CFLAGS += -I../src -L. -lm
LIBS :=-lc-goertzel

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

all: test

test: test.o libc-goertzel.a
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

libc-goertzel.a: $(C_GOERTZEL_PATH)/goertzel.o $(C_GOERTZEL_PATH)/goertzel-dtmf.o
	$(AR) rcs $@ $^

libc-goertzel-clean:
	rm -f $(C_GOERTZEL_PATH)/goertzel.o $(C_GOERTZEL_PATH)/goertzel-dtmf.o libc-goertzel.a

libs: libc-goertzel.a

.PHONY: clean

clean: libc-goertzel-clean
	rm -f test.o test
