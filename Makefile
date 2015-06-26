CC=g++
LIBS=-L/usr/local/lib -lprotobuf
CFLAGS=-c -Wall

all:

clean:
	rm -f bin/*

all_tests: classic_test complex_test node_test error_test matrix_test register_test gate_test entangledPair_test entangledRegister_test

protocol_buffers:
	$(CC) $(CFLAGS) $(LIBS) quantumMessage.pb.cc -o bin/quantumMessage.o

classic_test: clean
	$(CC) classic.cpp tests/classic_test.cpp -o bin/classic_test

complex_test: clean
	$(CC) complex.cpp tests/complex_test.cpp -o bin/complex_test

node_test: clean
	$(CC) complex.cpp node.cpp tests/node_test.cpp -o bin/node_test

error_test: clean
	$(CC) error.cpp tests/error_test.cpp -o bin/error_test

matrix_test: clean protocol_buffers
	$(CC) $(LIBS) bin/quantumMessage.o complex.cpp error.cpp system.cpp matrix.cpp tests/matrix_test.cpp -o bin/matrix_test

entangledPair_test: clean
	$(CC) complex.cpp error.cpp system.cpp matrix.cpp -g entangledPair.cpp -g tests/entangledPair_test.cpp -o bin/entangledPair_test	

register_test: clean
	$(CC) $(LIBS) complex.cpp error.cpp system.cpp matrix.cpp node.cpp register.cpp tests/register_test.cpp -o bin/register_test

gate_test: clean
	$(CC) complex.cpp error.cpp system.cpp matrix.cpp node.cpp register.cpp -g gates.cpp tests/gate_test.cpp -o bin/gate_test

entangledRegister_test: clean
	$(CC) -g complex.cpp error.cpp system.cpp matrix.cpp node.cpp -g register.cpp -g entangledRegister.cpp entanglement.cpp entangledPair.cpp tests/entangledRegister_test.cpp -o bin/entangledRegister_test

entanglement_test: clean
	$(CC) complex.cpp error.cpp system.cpp matrix.cpp node.cpp register.cpp entangledRegister.cpp entanglement.cpp entangledPair.cpp tests/entanglement_test.cpp -o bin/entanglement_test
