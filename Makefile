PWD=$(shell pwd)

INCS=-I$(PWD)/includes -I$(PWD)/includes/gates -I$(PWD)/includes/evils
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

SOURCES=$(wildcard src/*.cpp src/gates/*.cpp src/evils/*.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

RUN_SOURCES=$(wildcard runnables/src/*.cpp)
RUN_OBJECTS=$(patsubst %.cpp, %.o, $(RUN_SOURCES))

EXTERNAL_SOURCES=$(wildcard src/externals/*.cpp)
EXTERNAL_OBJECTS=$(patsubst %.cpp, %.o, $(EXTERNAL_SOURCES))
EXTERNAL_INCS=-I$(PWD)/includes/externals

all:

clean:
	-rm src/*.o bin/* src/*.pb.cc includes/*.pb.h $(O_PB) $(O_PB_GRPC) $(O_LIBQ) includes/gates.h runnables/src/*.o src/gates/*.o

prep:
	cat includes/gates/*.h > includes/gates.h

protocol_buffers: prep
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
	$(CC) $(C11_LIBFLAGS) $(INCS) $(LIBS) $(RUN_INCS) $(EXTERNAL_INCS) \
		-c $< \
		-o $@

runnables: $(RUN_OBJECTS)

$(RUN_OBJECTS): runnables/src/%.o :runnables/src/%.cpp
	$(CC) $(C11_LIBFLAGS) $(INCS) $(LIBS) $(RUN_INCS) $(EXTERNAL_INCS) \
		-c $< \
		-o $@

externals: $(EXTERNAL_OBJECTS)

$(EXTERNAL_OBJECTS): src/externals/%.o :src/externals/%.cpp
	$(CC) $(C11_LIBFLAGS) $(INCS) $(LIBS) $(EXTERNAL_INCS) \
		-c $< \
		-o $@

lib_qoosim: protocol_buffers $(OBJECTS) $(RUN_OBJECTS) $(EXTERNAL_OBJECTS)
	$(LINK) rcs $(O_QOOSIM) \
		src/*.o \
		src/evils/*.o \
		src/externals/*.o \
		src/gates/*.o \
		runnables/src/*.o

test_matrix: lib_qoosim
	$(CC) $(CFLAGS) $(INCS) $(LIBS) \
	tests/test_matrix.cpp \
 	$(O_QOOSIM) \
	$(O_PB) $(O_PB_GRPC) \
	-o bin/test_matrix

test_stateVector: lib_qoosim
	$(CC) $(CFLAGS) $(INCS) $(LIBS) \
	tests/test_stateVector.cpp \
 	$(O_QOOSIM) \
	$(O_PB) $(O_PB_GRPC) \
	-o bin/test_stateVector

test_qubitMap: lib_qoosim
	$(CC) $(CFLAGS) $(INCS) $(LIBS) \
	tests/test_qubitMap.cpp \
 	$(O_QOOSIM) \
	$(O_PB) $(O_PB_GRPC) \
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

test_system: lib_qoosim
	$(CC) $(CFLAGS) $(INCS) $(LIBS) $(RUN_INCS) \
	tests/test_echoServerRunnable.cpp \
 	$(O_QOOSIM) \
	$(O_PB) $(O_PB_GRPC) \
	-o bin/test_echoServerRunnable

	$(CC) $(CFLAGS) $(INCS) $(LIBS) $(RUN_INCS) \
	tests/test_echoClientRunnable.cpp \
 	$(O_QOOSIM) \
	$(O_PB) $(O_PB_GRPC) \
	-o bin/test_echoClientRunnable

test_teleportation: lib_qoosim
	$(CC) $(CFLAGS) $(INCS) $(LIBS) $(RUN_INCS) \
	tests/test_teleportationServer.cpp \
 	$(O_QOOSIM) \
	$(O_PB) $(O_PB_GRPC) \
	-o bin/test_teleportationServer

	$(CC) $(CFLAGS) $(INCS) $(LIBS) $(RUN_INCS) \
	tests/test_teleportationClient.cpp \
 	$(O_QOOSIM) \
	$(O_PB) $(O_PB_GRPC) \
	-o bin/test_teleportationClient

test_bb84: lib_qoosim
	$(CC) $(CFLAGS) $(INCS) $(LIBS) $(RUN_INCS) \
	tests/test_bb84Determiner.cpp \
 	$(O_QOOSIM) \
	$(O_PB) $(O_PB_GRPC) \
	-o bin/test_bb84Determiner

	$(CC) $(CFLAGS) $(INCS) $(LIBS) $(RUN_INCS) \
	tests/test_bb84Generator.cpp \
 	$(O_QOOSIM) \
	$(O_PB) $(O_PB_GRPC) \
	-o bin/test_bb84Generator

test_kak: clean lib_qoosim
	$(CC) $(CFLAGS) $(INCS) $(LIBS) $(RUN_INCS) \
	tests/test_kakinitiator.cpp \
 	$(O_QOOSIM) \
	$(O_PB) $(O_PB_GRPC) \
	-o bin/test_kakInitiator

	$(CC) $(CFLAGS) $(INCS) $(LIBS) $(RUN_INCS) \
	tests/test_kakresponder.cpp \
 	$(O_QOOSIM) \
	$(O_PB) $(O_PB_GRPC) \
	-o bin/test_kakResponder

test_tele: lib_qoosim
	$(CC) $(CFLAGS) $(INCS) $(LIBS) $(RUN_INCS) \
	tests/test_teleportation.cpp \
 	$(O_QOOSIM) \
	$(O_PB) $(O_PB_GRPC) \
	-o bin/test_teleportation
