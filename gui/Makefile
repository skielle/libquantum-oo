PWD=$(shell pwd)

INCS=-I$(PWD)
LIBS=-L/usr/local/lib -lgrpc++_unsecure -lgpr -lprotobuf -lpthread -ldl -lssl
GTK_FLAGS=$(shell pkg-config --cflags gtk+-3.0 vte-2.90)
GTK_LIBS=$(shell pkg-config --libs gtk+-3.0 vte-2.90)
GTK_CC=gcc

CC=g++
C_LIBFLAGS=-c 
CFLAGS= 
C11_LIBFLAGS=-g -c -std=c++11
C11_FLAGS=-g -std=c++11
LINK=ar

O_PB=../bin/gui.o
O_PB_GRPC=../bin/guiService.grpc.o

all:

clean:
	-rm *.o $(O_PB) $(O_PB_GRPC)

protocol_buffers:
	protoc --cpp_out=. gui.proto
	$(CC) $(C_LIBFLAGS) $(LIBS) gui.pb.cc -o $(O_PB)
	protoc --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` \
		 gui.proto
	$(CC) $(C11_LIBFLAGS) $(LIBS) -g gui.grpc.pb.cc \
		-o $(O_PB_GRPC) 

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
