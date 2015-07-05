CC=g++
LIBS=-L/usr/local/lib -lprotobuf
CFLAGS=-c -Wall
C11FLAGS=-c -Wall -std=c++11

all:

clean:
	rm -f bin/*

all_tests: classic_test complex_test node_test error_test matrix_test register_test gate_test entangledPair_test entangledRegister_test

protocol_buffers:
	protoc --cpp_out=. quantumMessage.proto
	$(CC) $(CFLAGS) $(LIBS) quantumMessage.pb.cc -o bin/quantumMessage.o
	protoc --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` \
		 quantumMessage.proto
	$(CC) $(C11FLAGS) $(LIBS) quantumMessage.grpc.pb.cc \
		-o bin/quantumMessage.grpc.o

channel_source: protocol_buffers
	$(CC) -std=c++11 bin/quantumMessage.o bin/quantumMessage.grpc.o -o bin/channelSource -L/usr/local/lib -lgrpc++_unsecure -lgrpc -lgpr -lprotobuf -lpthread -ldl complex.cpp channelService.cpp channelListener.cpp system.cpp error.cpp matrix.cpp node.cpp -g register.cpp gates.cpp channelSource.cpp 

channel_sink: protocol_buffers
	$(CC) -std=c++11 bin/quantumMessage.o bin/quantumMessage.grpc.o -o bin/channelSink -L/usr/local/lib -lgrpc++_unsecure -lgrpc -lgpr -lprotobuf -lpthread -ldl complex.cpp system.cpp error.cpp matrix.cpp node.cpp -g register.cpp channelSink.cpp 

channelListener_test: protocol_buffers channel_source
	$(CC) -std=c++11 bin/quantumMessage.o bin/quantumMessage.grpc.o -o bin/channelListener_test -L/usr/local/lib -lgrpc++_unsecure -lgrpc -lgpr -lprotobuf -lpthread -ldl complex.cpp system.cpp error.cpp matrix.cpp node.cpp -g register.cpp channelListener.cpp channelService.cpp tests/channelListener_test.cpp

channel: channel_source channel_sink

classic_test: clean
	$(CC) classic.cpp tests/classic_test.cpp -o bin/classic_test

classic_register_test: clean protocol_buffers
	$(CC) $(LIBS) bin/quantumMessage.o classic.cpp error.cpp classicRegister.cpp tests/classicRegister_test.cpp -o bin/classicRegister_test

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

entangledRegister_test: clean protocol_buffers
	$(CC) $(LIBS) bin/quantumMessage.o -g complex.cpp error.cpp system.cpp matrix.cpp node.cpp gates.cpp -g register.cpp -g entangledRegister.cpp entanglement.cpp entangledPair.cpp tests/entangledRegister_test.cpp -o bin/entangledRegister_test

entanglement_test: clean
	$(CC) complex.cpp error.cpp system.cpp matrix.cpp node.cpp register.cpp entangledRegister.cpp entanglement.cpp entangledPair.cpp tests/entanglement_test.cpp -o bin/entanglement_test

system_test: protocol_buffers
	$(CC) -std=c++11 -L/usr/local/lib -lgrpc++_unsecure -lgrpc -lgpr -lprotobuf -lpthread -ldl bin/quantumMessage.o bin/quantumMessage.grpc.o complex.cpp error.cpp channelService.cpp channelListener.cpp -g system.cpp matrix.cpp node.cpp register.cpp tests/system_test.cpp -o bin/system_test
