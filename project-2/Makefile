
CC=gcc
CCOPTS=-Wall -Wextra
LIBS=

SRCS=$(wildcard *.c)
TARGETS=$(SRCS:.c=)

.PHONY: all clean

.SILENT:
all: $(TARGETS)

clean:
	rm -f $(TARGETS)

run:
	make clean
	make all
	./hello
	make clean

%: %.c
	$(CC) $(CCOPTS) -o $@ $< $(LIBS)