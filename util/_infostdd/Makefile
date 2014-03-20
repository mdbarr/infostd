CFLAGS = -O2 -g -Wall -I../../src -L../../src

all: _infostdd

_infostdd: _infostdd.c ../../src/libinfostd.a
	gcc $(CFLAGS) -o _infostdd _infostdd.c ../../src/libinfostd.a

clean:
	rm -f core *~ *.o _infostdd
