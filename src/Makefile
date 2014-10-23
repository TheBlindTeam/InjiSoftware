CC=clang

CFL=-Wall -Werror -Wextra -std=c99 -O2 -rdynamic `pkg-config gtk+-3.0 --cflags`

LDFLAGS=`pkg-config gtk+-3.0 --libs`

LDLIBS=-lm

SRC=main.c Interface/*.c PreTreatment/matrixOperation.c Utils/*.c NeuralNetwork/*.c

all:
	$(CC) $(CFL) $(SRC) $(LDFLAGS) $(LDLIBS) -o Inji

clean:
	rm -f Inji
