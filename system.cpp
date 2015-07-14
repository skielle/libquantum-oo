#include <memory>
#include <thread>
#include <queue>
#include <unistd.h>
#include <utility>

#include "channelListener.h"
#include "classicRegister.h"
#include "echoRunnable.h"
#include "gates.cpp"
#include "system.h"
#include "register.h"

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

System::System () :
	server(new QuantumChannel::ChannelListener)
{
	shared_ptr<iRunnable> a ( new EchoRunnable() );
	this->algorithm = a;
	this->server->setPort(50051);
}

void System::RunSystem() {
	thread serverThread = thread(&System::runServer, this);
	thread algorithmThread = thread(&System::runAlgorithm, this);
	serverThread.join();
	algorithmThread.join();
}

void System::runServer() {
	this->server->Run();
}

void System::setAlgorithm(shared_ptr<iRunnable> a) {
	this->algorithm = a;
}

void System::runAlgorithm() {
	this->algorithm->Run();
}

System* System::getInstance() {
	if ( System::systemInstance == 0 ) {
		System::systemInstance = new System();
	}
	return System::systemInstance;
}

bool System::isMessageQueueEmpty() {
	return this->messageQueue.empty();
}

SystemMessage System::getMessageType() {
	return this->messageQueue.front().first;
}

int System::addRegister(shared_ptr<iRegister> reg, SystemMessage message) {
	registers.push_back(reg);
	messageQueue.push(
		pair<SystemMessage, int>(message, registers.size() -1));
	return registers.size() - 1;
}

shared_ptr<iRegister> System::getRegister(int hash) {
	return registers.at(hash);
}

int System::getListenerPort() {
	return this->server->getPort();
}

void System::setListenerPort(int port) {
	this->server->setPort(port);
}

}
