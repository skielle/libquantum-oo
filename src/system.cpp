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

}
