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
	shared_ptr<Qubit> q1 = Qubit::create();
	shared_ptr<Qubit> q2 = Qubit::create();
	shared_ptr<Qubit> psi = Qubit::create();
	
	Hadamard h;
	Ry r(M_PI/3);
	CNot cn;
	
	System* sys = System::getInstance();
	QuantumChannel::ChannelService_client csc(this->serverIP, this->serverPort);
	psi->applyMatrix(r);
	psi->print();

	vector< shared_ptr<Qubit> > inputs;
	inputs.push_back(q1);
	inputs.push_back(q2);
	q1->applyMatrix(h);
	q1->v->applyOperation(cn, inputs);
	csc.SendQubit(q2);

	sleep(2);

	inputs.at(0) = psi;
	inputs.at(1) = q1;
	q1->v->applyOperation(cn, inputs);
	psi->applyMatrix(h);

	int q1_result = q1->measure();
	int psi_result = psi->measure();
	printf("q1: %i, psi: %i\r\n", q1_result, psi_result);

	sys->stopServer();
}
}
