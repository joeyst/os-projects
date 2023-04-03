CC=gcc
CCOPTS=-Wall -Wextra -Werror
LIBS=

SRCS=$(wildcard *.c)
TARGETS=$(SRCS:.c=)

.PHONY: all clean

all: $(TARGETS)

clean:
	rm -f $(TARGETS)

debug:
	gcc -g ptsim.c -o ptsim

%: %.c
	$(CC) $(CCOPTS) -o $@ $< $(LIBS)
    
