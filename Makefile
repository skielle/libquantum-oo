CC=g++
CFLAGS=-c -Wall

all:

all_tests: classic_test complex_test node_test error_test

classic_test: 
	$(CC) classic.cpp classic_test.cpp -o classic_test

classic.o:
	$(CC) $(CFLAGS) classic.cpp

complex_test:
	$(CC) complex.cpp complex_test.cpp -o complex_test

complex.o:
	$(CC) $(CFLAGS) complex.cpp

node_test:
	$(CC) node.cpp node_test.cpp -o node_test

node.o:
	$(CC) $(CFLAGS) node.cpp

error_test:
	$(CC) error.cpp error_test.cpp -o error_test

error.o:
	$(CC) $(CFLAGS) error.cpp
