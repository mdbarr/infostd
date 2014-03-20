CFLAGS = -O2 -g -Wall -I../../src -L../../src

all: spellcheck

spellcheck: spellcheck.c ../../src/libinfostd.a
	gcc $(CFLAGS) -o spellcheck spellcheck.c ../../src/libinfostd.a

clean:
	rm -f core *~ *.o spellcheck
