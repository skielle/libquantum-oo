PWD=$(shell pwd)

INCS=-I$(PWD)/includes
LIBS=-L/usr/local/lib -L$(PWD)/src -lcrypto -lgrpc_unsecure -lgrpc++_unsecure -lgpr -lprotobuf -lpthread -ldl -lssl
GTK_FLAGS=$(shell pkg-config --cflags gtk+-3.0 vte-2.90)
GTK_LIBS=$(shell pkg-config --libs gtk+-3.0 vte-2.90)
GTK_CC=gcc

RUN_INCS=-I$(PWD)/runnables/includes

CC=g++
C_LIBFLAGS=-c 
CFLAGS=-Wall -Wno-sign-compare -g -std=c++11
C11_LIBFLAGS=-g -c -std=c++11 -fext-numeric-literals
C11_FLAGS=-g -std=c++11 -fext-numeric-literals
LINK=ar

O_PB=bin/quantumMessage.o
O_PB_GRPC=bin/quantumMessage.grpc.o
O_QOOSIM=bin/lib_qoosim.a

SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

all:

clean:
	-rm src/*.o bin/* src/*.pb.cc includes/*.pb.h $(O_PB) $(O_PB_GRPC) $(O_LIBQ)

protocol_buffers: clean
	cd resources; protoc --cpp_out=. quantumMessage.proto
	mv resources/*.h includes
	mv resources/*.cc src
	$(CC) $(C11_LIBFLAGS) $(INCS) $(LIBS) src/quantumMessage.pb.cc -o $(O_PB)
	cd resources; protoc --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` \
		quantumMessage.proto
	mv resources/*.h includes
	mv resources/*.cc src
	$(CC) $(C11_LIBFLAGS) $(INCS) $(LIBS) src/quantumMessage.grpc.pb.cc \
		-o $(O_PB_GRPC)

$(OBJECTS): src/%.o :src/%.cpp
	$(CC) $(C11_LIBFLAGS) $(INCS) $(LIBS) \
		-c $< \
		-o $@

lib_qoosim: protocol_buffers $(OBJECTS)
	$(LINK) rcs $(O_QOOSIM) \
		src/complex.o \
		src/error.o \
		src/matrix.o \
		src/stateVector.o \
		src/qubitMap.o \
		src/qubit.o \
		src/channelListener.o \
		src/channelService.o \
		src/channelService_client.o \
		src/remotePeer.o \
		src/channel.o \
		src/system.o \
		src/echoRunnable.o \
		src/echoClientRunnable.o \
		src/stateVectorOperation.o

test_matrix: lib_qoosim
	$(CC) $(CFLAGS) $(INCS) $(LIBS) \
	tests/test_matrix.cpp \
 	$(O_QOOSIM) \
	-o bin/test_matrix

test_stateVector: lib_qoosim
	$(CC) $(CFLAGS) $(INCS) $(LIBS) \
	tests/test_stateVector.cpp \
 	$(O_QOOSIM) \
	-o bin/test_stateVector

test_qubitMap: lib_qoosim
	$(CC) $(CFLAGS) $(INCS) $(LIBS) \
	tests/test_qubitMap.cpp \
 	$(O_QOOSIM) \
	-o bin/test_qubitMap 

test_qubit: lib_qoosim
	$(CC) $(CFLAGS) $(INCS) $(LIBS) \
	tests/test_qubit.cpp \
 	$(O_QOOSIM) \
	$(O_PB) $(O_PB_GRPC) \
	-o bin/test_qubit 

	$(CC) $(CFLAGS) $(INCS) $(LIBS) \
	tests/test_two_qubits.cpp \
 	$(O_QOOSIM) \
	$(O_PB) $(O_PB_GRPC) \
	-o bin/test_two_qubits 

	$(CC) $(CFLAGS) $(INCS) $(LIBS) \
	tests/test_syndrome.cpp \
 	$(O_QOOSIM) \
	$(O_PB) $(O_PB_GRPC) \
	-o bin/test_syndrome

	$(CC) $(CFLAGS) $(INCS) $(LIBS) \
	tests/test_two_qubit_syndrome.cpp \
 	$(O_QOOSIM) \
	$(O_PB) $(O_PB_GRPC) \
	-o bin/test_two_qubit_syndrome

test_channel: clean lib_qoosim
	$(CC) $(CFLAGS) $(INCS) $(LIBS) \
	tests/test_listener.cpp \
 	$(O_QOOSIM) \
	$(O_PB) $(O_PB_GRPC) \
	-o bin/test_listener

	$(CC) $(CFLAGS) $(INCS) $(LIBS) \
	tests/test_client.cpp \
 	$(O_QOOSIM) \
	$(O_PB) $(O_PB_GRPC) \
	-o bin/test_client

test_system: clean lib_qoosim
	$(CC) $(CFLAGS) $(INCS) $(LIBS) \
	tests/test_echoServerRunnable.cpp \
 	$(O_QOOSIM) \
	$(O_PB) $(O_PB_GRPC) \
	-o bin/test_echoServerRunnable

	$(CC) $(CFLAGS) $(INCS) $(LIBS) \
	tests/test_echoClientRunnable.cpp \
 	$(O_QOOSIM) \
	$(O_PB) $(O_PB_GRPC) \
	-o bin/test_echoClientRunnable
