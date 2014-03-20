CFLAGS = -O2 -g -Wall -I../src -L../src

all:
	cd _infostdd; make
	cd metaphone; make
	cd spellcheck; make
	cd stemmer; make

clean:
	cd _infostdd; make clean
	cd metaphone; make clean
	cd spellcheck; make clean
	cd stemmer; make clean
