CC=g++
CFLAGS=-c -Wall

all:

all_tests: classic_test complex_test node_test error_test matrix_test register_test

classic_test: 
	$(CC) classic.cpp classic_test.cpp -o classic_test

complex_test:
	$(CC) complex.cpp complex_test.cpp -o complex_test

node_test:
	$(CC) node.cpp node_test.cpp -o node_test

error_test:
	$(CC) error.cpp error_test.cpp -o error_test

matrix_test:
	$(CC) complex.cpp error.cpp matrix.cpp matrix_test.cpp -o matrix_test

register_test:
	$(CC) complex.cpp error.cpp matrix.cpp node.cpp register.cpp register_test.cpp -o register_test

gate_test:
	$(CC) complex.cpp error.cpp matrix.cpp node.cpp register.cpp -g gates.cpp gate_test.cpp -o gate_test
