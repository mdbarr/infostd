CFLAGS = -O2 -g -Wall -I../../src -L../../src

all: stemmer

stemmer: stemmer.c ../../src/libinfostd.a
	gcc $(CFLAGS) -o stemmer stemmer.c ../../src/libinfostd.a

clean:
	rm -f core *~ *.o stemmer
