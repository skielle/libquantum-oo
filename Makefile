PWD=$(shell pwd)

INCS=-I$(PWD)
LIBS=-L/usr/local/lib -lgrpc++_unsecure -lgpr -lprotobuf -lpthread -ldl -lssl

CC=g++
C_LIBFLAGS=-c 
CFLAGS= 
C11_LIBFLAGS=-c -std=c++11
C11_FLAGS=-std=c++11
LINK=ar

O_PB=bin/quantumMessage.o
O_PB_GRPC=bin/quantumMessage.grpc.o
O_LIBQ=bin/lib_quantum_oo.a

all:

clean:
	rm *.o $(O_PB) $(O_PB_GRPC) $(O_LIBQ)

all_tests: classic_test complex_test node_test error_test matrix_test register_test gate_test entangledPair_test entangledRegister_test

protocol_buffers:
	protoc --cpp_out=. quantumMessage.proto
	$(CC) $(C_LIBFLAGS) $(LIBS) quantumMessage.pb.cc -o $(O_PB)
	protoc --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` \
		 quantumMessage.proto
	$(CC) $(C11_LIBFLAGS) $(LIBS) -g quantumMessage.grpc.pb.cc \
		-o $(O_PB_GRPC) 

classic_test: clean
	$(CC) classic.cpp tests/classic_test.cpp -o bin/classic_test

classic_register_test: clean protocol_buffers
	$(CC) $(LIBS) bin/quantumMessage.o classic.cpp error.cpp -g classicRegister.cpp tests/classicRegister_test.cpp -o bin/classicRegister_test

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
	$(CC) -std=c++11 -L/usr/local/lib -lgrpc++_unsecure -lgrpc -lgpr -lprotobuf -lpthread -ldl bin/quantumMessage.o bin/quantumMessage.grpc.o classic.cpp complex.cpp error.cpp channelService.cpp channelListener.cpp channelService_client.cpp echoRunnable.cpp -g system.cpp matrix.cpp node.cpp classicRegister.cpp register.cpp tests/system_test.cpp -o bin/system_test

libquantum-oo: protocol_buffers
	$(CC) $(C11_LIBFLAGS) $(INCS) $(LIBS) $(O_PB) $(O_PB_GRPC) \
		classic.cpp \
		complex.cpp \
		error.cpp \
		systemMessage.cpp \
		channelService.cpp \
		channelListener.cpp \
		channelService_client.cpp \
		echoRunnable.cpp \
		system.cpp \
		matrix.cpp \
		node.cpp \
		classicRegister.cpp \
		register.cpp \
		entangledRegister.cpp \
		entanglement.cpp \
		entangledPair.cpp 
	$(LINK) rcs $(O_LIBQ) \
		$(O_PB) $(O_PB_GRPC) \
		classic.o \
		complex.o \
		error.o \
		systemMessage.o \
		channelService.o \
		channelListener.o \
		channelService_client.o \
		echoRunnable.o \
		system.o \
		matrix.o \
		node.o \
		classicRegister.o \
		register.o \
		entangledRegister.o \
		entanglement.o \
		entangledPair.o 

bb84: libquantum-oo 
	$(CC) $(C11_FLAGS) $(INCS) $(LIBS) $(O_PB) $(O_PB_GRPC) \
		runnables/bb84/bb84Util.cpp \
		runnables/bb84/bb84Determination_runnable.cpp \
		runnables/bb84/bb84Determination.cpp \
		-o bin/bb84Determination \
		$(O_LIBQ)
	$(CC) $(C11_FLAGS) $(INCS) $(LIBS) $(O_PB) $(O_PB_GRPC) \
		runnables/bb84/bb84Util.cpp \
		runnables/bb84/bb84Generation_runnable.cpp \
		runnables/bb84/bb84Generation.cpp \
		-o bin/bb84Generation \
		$(O_LIBQ)
