#include <memory>
#include <thread>
#include <queue>
#include <unistd.h>
#include <utility>

#include "channelListener.h"
#include "echoRunnable.h"
#include "system.h"

namespace Quantum {

System* System::systemInstance = 0;

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

void System::stopServer() {
	this->server->Stop();
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

int System::getListenerPort() {
	return this->server->getPort();
}

void System::setListenerPort(int port) {
	this->server->setPort(port);
}

void System::addMessage(int address) {
	this->messageQueue.push(
		pair<SystemMessage, int> (
			SystemMessage(SystemMessage::QUANTUM_DATA_RECEIVED),
			address) );
}

void System::addMessage(string data) {
	this->classicData.push_back(data);
	this->messageQueue.push(
		pair<SystemMessage, int> (
			SystemMessage(SystemMessage::CLASSIC_DATA_RECEIVED),
			this->classicData.size() - 1 ) );
}

bool System::isMessageQueueEmpty() {
	return this->messageQueue.empty();
}

SystemMessage System::nextMessageType() {
	return this->messageQueue.front().first;
}

int System::nextMessage() {
	int address = this->messageQueue.front().second;
	this->messageQueue.pop();
	return address;
}

string System::getClassicData(int address) {
	return this->classicData.at(address);
}

}
