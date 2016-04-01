/*
 * echoClientRunnable.cpp
 */
#include <memory>
#include <unistd.h>
#include <utility>
#include <stdio.h>
#include <string>
#include <vector>

#include "echoClientRunnable.h"
#include "system.h"
#include "remotePeer.h"
#include "qubit.h"
#include "gates.h"
#include "qubitMap.h"
#include "channelService_client.h"

using namespace std;

namespace Quantum {
void EchoClientRunnable::Run() {
	int i;

	Hadamard h;
	
	Matrix cnot = Matrix(4,4);
	cnot.set(0, 0, 1);
	cnot.set(1, 1, 1);
	cnot.set(2, 3, 1);
	cnot.set(3, 2, 1);

	System* sys = System::getInstance();
	QuantumChannel::ChannelService_client csc(this->serverIP, this->serverPort);
	printf("ECHO CLIENT RUNNABLE\n");

	sleep(5);

	shared_ptr<Qubit> q1 = Qubit::create();
	shared_ptr<Qubit> q2 = Qubit::create();

	q1->applyMatrix(h);

	vector< shared_ptr<Qubit> > inputs;
	inputs.push_back(q1);
	inputs.push_back(q2);

	q1->v->applyOperation(cnot, inputs);
	q1->print();
	csc.SendQubit(q2);

	sleep(5);

	int q1_result = q1->measure();
	sleep(5);
	printf("q1: %i\r\n", q1_result);

	sys->stopServer();
}
}
