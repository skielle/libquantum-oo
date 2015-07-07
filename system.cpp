#include <memory>
#include <thread>
#include <queue>
#include <unistd.h>
#include <utility>
#include "register.h"
#include "gates.cpp"
#include "system.h"
#include "channelListener.h"
#include "channelService_client.h"

namespace Quantum {
long System::mem = 0;
long System::memmax = 0;

System* System::systemInstance = 0;

unsigned long System::memman(long change) {
	System::mem += change;
	if ( System::mem > System::memmax ) {
		System::memmax = System::mem;
	}
	return System::mem;
}

System::System () {
}

void System::runServer() {
	QuantumChannel::ChannelListener cl;
	thread serverThread = thread(&QuantumChannel::ChannelListener::Run, 
		&cl, this->getListenerPort());
	thread algorithmThread = thread(&System::runAlgorithm, this);
	serverThread.join();
	algorithmThread.join();
}

void System::runAlgorithm() {
	sleep(5);
	Register myReg = Register((MAX_UNSIGNED)0, 4);
	myReg.applyGate(new Hadamard(), 0);
	myReg.applyGate(new Hadamard(), 3);

	QuantumChannel::ChannelService_client csc("127.0.0.1", 50050);
	csc.SendRegister(myReg);

	printf("RUNNING AN ALGORITHM NOW...");
	while ( true ) {
		sleep(1);
		if ( this->messageQueue.empty() ) {
			printf ("NO MESSAGES CAME IN\n");
		} else {
			if ( this->messageQueue.front().first ==
				SystemMessage::REGISTER_RECIEVED ) {
				this->getMessage<Register>();
				//Register rx = this->getMessage<Register>();
				//rx.print();
			}
		}
	}
}

System* System::getInstance() {
	if ( System::systemInstance == 0 ) {
		System::systemInstance = new System();
	}
	return System::systemInstance;
}

int System::addRegister(shared_ptr<iRegister> reg) {
	registers.push_back(reg);
	messageQueue.push(
		pair<SystemMessage, int>(
			SystemMessage::REGISTER_RECIEVED, 
			registers.size() -1));
	return registers.size() - 1;
}

shared_ptr<iRegister> System::getRegister(int hash) {
	return registers.at(hash);
}

int System::getListenerPort() {
	return this->listenerPort;
}

void System::setListenerPort(int port) {
	this->listenerPort = port;
}

}
