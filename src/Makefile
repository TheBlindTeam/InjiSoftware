CC=gcc

CFL=-Wall -Werror -Wextra -std=c99 -O2 -rdynamic

CFL2=-Wall -Werror -Wextra -std=c99

LDFLAGS= `pkg-config gtk+-3.0 --cflags`

LDLIBS=`pkg-config gtk+-3.0 --libs`-lm

SRC= */*.c *.c

all:
	$(CC) $(CFL) $(LDFLAGS) $(SRC) $(LDLIBS) -o Inji

debug:	
	$(CC) $(CFL2) $(LDFLAGS) $(SRC) $(LDLIBS) -o InjiDebug

clean:
	rm -f Inji InjiDebug
