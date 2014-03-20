CFLAGS = -O2 -g -Wall -I../src -L../src

all: tester

tester: tester.c ../src/libinfostd.a
	gcc $(CFLAGS) -o tester tester.c -linfostd

clean:
	rm -f core *~ *.o tester

test:	tester
	exec ./tester