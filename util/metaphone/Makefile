CFLAGS = -O2 -g -Wall -I../../src -L../../src

all: metaphone

metaphone: metaphone.c ../../src/libinfostd.a
	gcc $(CFLAGS) -o metaphone metaphone.c ../../src/libinfostd.a

clean:
	rm -f core *~ *.o metaphone
