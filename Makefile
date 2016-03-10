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
O_LIBQ=bin/lib_quantum_oo.a

all:

clean:
	-rm *.o $(O_PB) $(O_PB_GRPC) $(O_LIBQ)

test_matrix:
	$(CC) $(CFLAGS) $(INCS) $(LIBS) \
	src/error.cpp \
	src/matrix.cpp \
	tests/test_matrix.cpp \
	-o bin/test_matrix

test_stateVector:
	$(CC) $(CFLAGS) $(INCS) $(LIBS) \
	src/error.cpp \
	src/matrix.cpp \
	src/stateVector.cpp \
	tests/test_stateVector.cpp \
	-o bin/test_stateVector

test_qubitMap:
	$(CC) $(CFLAGS) $(INCS) $(LIBS) \
	src/error.cpp \
	src/matrix.cpp \
	src/stateVector.cpp \
	src/qubitMap.cpp \
	tests/test_qubitMap.cpp \
	-o bin/test_qubitMap 

test_qubit:
	$(CC) $(CFLAGS) $(INCS) $(LIBS) \
	src/error.cpp \
	src/matrix.cpp \
	src/stateVector.cpp \
	src/qubitMap.cpp \
	src/qubit.cpp \
	tests/test_qubit.cpp \
	-o bin/test_qubit 

	$(CC) $(CFLAGS) $(INCS) $(LIBS) \
	src/error.cpp \
	src/matrix.cpp \
	src/stateVector.cpp \
	src/qubitMap.cpp \
	src/qubit.cpp \
	tests/test_two_qubits.cpp \
	-o bin/test_two_qubits 

	$(CC) $(CFLAGS) $(INCS) $(LIBS) \
	src/error.cpp \
	src/matrix.cpp \
	src/stateVector.cpp \
	src/qubitMap.cpp \
	src/qubit.cpp \
	tests/test_syndrome.cpp \
	-o bin/test_syndrome

	$(CC) $(CFLAGS) $(INCS) $(LIBS) \
	src/error.cpp \
	src/matrix.cpp \
	src/stateVector.cpp \
	src/qubitMap.cpp \
	src/qubit.cpp \
	tests/test_two_qubit_syndrome.cpp \
	-o bin/test_two_qubit_syndrome

clean-gui:
	-rm bin/mainScreen

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

_matrix_test: libquantum-oo 
	$(CC) $(C11_FLAGS) $(INCS) $(LIBS) $(O_PB) $(O_PB_GRPC) \
		tests/matrix_test.cpp \
		-o bin/matrix_test \
		$(O_LIBQ)

entangledPair_test: clean
	$(CC) complex.cpp error.cpp system.cpp matrix.cpp -g entangledPair.cpp -g tests/entangledPair_test.cpp -o bin/entangledPair_test	


entangledRegister_test: libquantum-oo 
	$(CC) $(C11_FLAGS) $(INCS) $(LIBS) $(O_PB) $(O_PB_GRPC) \
		tests/entangledRegister_test.cpp \
		-o bin/entangledRegister_test \
		$(O_LIBQ)

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
