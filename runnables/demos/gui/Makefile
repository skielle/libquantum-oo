PWD=$(shell pwd)

INCS=-I$(PWD) -I../../..
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

O_PB=../../../bin/quantumMessage.o
O_PB_GRPC=../../../bin/quantumMessage.grpc.o
O_PB_G=../../../bin/gui.o
O_PB_G_GRPC=../../../bin/guiService.grpc.o
O_LIBQ=../../../bin/lib_quantum_oo.a

all:

clean:
	-rm *.o $(O_PB) $(O_PB_GRPC)

protocol_buffers:
	protoc --python_out=. --grpc_out=. \
		--plugin=protoc-gen-grpc=`which grpc_python_plugin` \
		gui.proto
	protoc --cpp_out=. gui.proto
	$(CC) $(C_LIBFLAGS) $(LIBS) gui.pb.cc -o $(O_PB_G)
	protoc --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` \
		 gui.proto
	$(CC) $(C11_LIBFLAGS) $(LIBS) -g gui.grpc.pb.cc \
		-o $(O_PB_G_GRPC) 

controllerService: protocol_buffers
	$(CC) $(C11_FLAGS) $(INCS) $(LIBS) \
		 $(O_PB) $(O_PB_GRPC) \
		 $(O_PB_G) $(O_PB_G_GRPC) \
		controllerListenerService.cpp \
		controllerListener.cpp \
		concreteController.cpp \
		controller.cpp \
		-o controllerTest \
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
