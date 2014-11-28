CC=gcc

CFL=-Wall -Werror -Wextra -std=c99 -O2 -rdynamic

LDFLAGS= `pkg-config gtk+-3.0 --cflags`

LDLIBS=`pkg-config gtk+-3.0 --libs`-lm

SRC= */*.c *.c

all:
	$(CC) $(CFL) $(LDFLAGS) $(SRC) $(LDLIBS) -o Inji

clean:
	rm -f Inji
