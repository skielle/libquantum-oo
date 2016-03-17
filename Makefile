PWD=$(shell pwd)

INCS=-I$(PWD)/includes
LIBS=-L/usr/local/lib -L$(PWD)/src -lcrypto -lgrpc_unsecure -lgrpc++_unsecure -lgpr -lprotobuf -lpthread -ldl -lssl
GTK_FLAGS=$(shell pkg-config --cflags gtk+-3.0 vte-2.90)
GTK_LIBS=$(shell pkg-config --libs gtk+-3.0 vte-2.90)
GTK_CC=gcc

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
		src/remoteVectorMap.o \
		src/remotePeer.o \
		src/channel.o \
		src/system.o \
		src/echoRunnable.o	

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
	tests/test_runnableEchoServer.cpp \
 	$(O_QOOSIM) \
	$(O_PB) $(O_PB_GRPC) \
	-o bin/test_runnableEchoServer

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
		silentEvil.cpp \
		system.cpp \
		matrix.cpp \
		node.cpp \
		qubit.cpp \
		classicRegister.cpp \
		register.cpp \
		entangledRegister.cpp \
		stub__networkRegister.cpp \
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
		silentEvil.o \
		system.o \
		matrix.o \
		node.o \
		qubit.o \
		classicRegister.o \
		register.o \
		entangledRegister.o \
		stub__networkRegister.o \
		entanglement.o \
		entangledPair.o 

bb84-classic: libquantum-oo 
	$(CC) $(C11_FLAGS) $(INCS) $(LIBS) $(O_PB) $(O_PB_GRPC) \
		runnables/bb84-classic/bb84Util.cpp \
		runnables/bb84-classic/bb84Determination_runnable.cpp \
		runnables/bb84-classic/bb84Determination.cpp \
		-o bin/bb84-classic_Determination \
		$(O_LIBQ)
	$(CC) $(C11_FLAGS) $(INCS) $(LIBS) $(O_PB) $(O_PB_GRPC) \
		runnables/bb84-classic/bb84Util.cpp \
		runnables/bb84-classic/bb84Generation_runnable.cpp \
		runnables/bb84-classic/bb84Generation.cpp \
		-o bin/bb84-classic_Generation \
		$(O_LIBQ)

bb84-polybase: libquantum-oo 
	$(CC) $(C11_FLAGS) $(INCS) $(LIBS) $(O_PB) $(O_PB_GRPC) \
		runnables/bb84-polybase/bb84Util.cpp \
		runnables/bb84-polybase/bb84Determination_runnable.cpp \
		runnables/bb84-polybase/bb84Determination.cpp \
		-o bin/bb84-polybase_Determination \
		$(O_LIBQ)
	$(CC) $(C11_FLAGS) $(INCS) $(LIBS) $(O_PB) $(O_PB_GRPC) \
		runnables/bb84-polybase/bb84Util.cpp \
		runnables/bb84-polybase/bb84Generation_runnable.cpp \
		runnables/bb84-polybase/bb84Generation.cpp \
		-o bin/bb84-polybase_Generation \
		$(O_LIBQ)

bb84-abcl98: libquantum-oo 
	$(CC) $(C11_FLAGS) $(INCS) $(LIBS) $(O_PB) $(O_PB_GRPC) \
		runnables/bb84-abcl98/bb84Util.cpp \
		runnables/bb84-abcl98/bb84Determination_runnable.cpp \
		runnables/bb84-abcl98/bb84Determination.cpp \
		-o bin/bb84-abcl98_Determination \
		$(O_LIBQ)
	$(CC) $(C11_FLAGS) $(INCS) $(LIBS) $(O_PB) $(O_PB_GRPC) \
		runnables/bb84-abcl98/bb84Util.cpp \
		runnables/bb84-abcl98/bb84Generation_runnable.cpp \
		runnables/bb84-abcl98/bb84Generation.cpp \
		-o bin/bb84-abcl98_Generation \
		$(O_LIBQ)

kak06: libquantum-oo
	$(CC) $(C11_FLAGS) $(INCS) $(LIBS) $(O_PB) $(O_PB_GRPC) \
		runnables/kak/kakUtil.cpp \
		runnables/kak/kakParty_runnable.cpp \
		runnables/kak/kakFulfiller.cpp \
		-o bin/kakFulfiller \
		$(O_LIBQ)
	$(CC) $(C11_FLAGS) $(INCS) $(LIBS) $(O_PB) $(O_PB_GRPC) \
		runnables/kak/kakUtil.cpp \
		runnables/kak/kakParty_runnable.cpp \
		runnables/kak/kakInstigator.cpp \
		-o bin/kakInstigator \
		$(O_LIBQ)
	
demo-entanglement: libquantum-oo
	$(CC) $(C11_FLAGS) $(INCS) $(LIBS) $(O_PB) $(O_PB_GRPC) \
		runnables/demo-entanglement/sink.cpp \
		runnables/demo-entanglement/sink_runnable.cpp \
		-o bin/demo-entanglement_sink \
		$(O_LIBQ)
	$(CC) $(C11_FLAGS) $(INCS) $(LIBS) $(O_PB) $(O_PB_GRPC) \
		runnables/demo-entanglement/source.cpp \
		runnables/demo-entanglement/source_runnable.cpp \
		-o bin/demo-entanglement_source \
		$(O_LIBQ)
	
qubit_test: libquantum-oo 
	$(CC) $(C11_FLAGS) $(INCS) $(LIBS) $(O_PB) $(O_PB_GRPC) \
		tests/qubit_test.cpp \
		-o bin/qubit_test \
		$(O_LIBQ)

gate_test: libquantum-oo 
	$(CC) $(C11_FLAGS) $(INCS) $(LIBS) $(O_PB) $(O_PB_GRPC) \
		tests/gate_test.cpp \
		-o bin/gate_test \
		$(O_LIBQ)

register_test: libquantum-oo 
	$(CC) $(C11_FLAGS) $(INCS) $(LIBS) $(O_PB) $(O_PB_GRPC) \
		tests/register_test.cpp \
		-o bin/register_test \
		$(O_LIBQ)

demo_superposition_1: libquantum-oo 
	$(CC) $(C11_FLAGS) $(INCS) $(LIBS) $(O_PB) $(O_PB_GRPC) \
		runnables/demos/superposition_1.cpp \
		runnables/demos/superposition_1_runnable.cpp \
		-o bin/demo-superposition_1 \
		$(O_LIBQ)

demo_superposition_2: libquantum-oo 
	$(CC) $(C11_FLAGS) $(INCS) $(LIBS) $(O_PB) $(O_PB_GRPC) \
		runnables/demos/superposition_2.cpp \
		runnables/demos/superposition_2_runnable.cpp \
		-o bin/demo-superposition_2 \
		$(O_LIBQ)

gui: clean-gui
	$(GTK_CC) $(GTK_FLAGS) -o bin/mainScreen -g gui/mainScreen.c $(GTK_LIBS)
