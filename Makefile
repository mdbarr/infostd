all:
	cd src; make
	cd tester; make
	cd util; make

install:
	cd src; make install

clean:
	cd src; make clean
	cd tester; make clean
	cd util; make clean

test:	
	cd tester; make test

