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
#include "qubitMap.h"
#include "channelService_client.h"

using namespace std;

namespace Quantum {
void EchoClientRunnable::Run() {
	int i;

	Matrix hadamard = Matrix(2,2);
	hadamard.set(0,0,1/sqrt(2));
	hadamard.set(0,1,1/sqrt(2));
	hadamard.set(1,0,-1/sqrt(2));
	hadamard.set(1,1,1/sqrt(2));

	Matrix cnot = Matrix(4,4);
	cnot.set(0, 0, 1);
	cnot.set(1, 1, 1);
	cnot.set(2, 3, 1);
	cnot.set(3, 2, 1);

	QuantumChannel::ChannelService_client csc(this->serverIP, this->serverPort);
	printf("ECHO CLIENT RUNNABLE\n");

	sleep(5);

	vector< shared_ptr<Qubit> > Q(8);

	for ( i = 0; i < Q.size(); i++ ) {
		Q.at(i) = Qubit::create();
		if ( i % 2 == 0 ) {
			Q.at(i)->applyMatrix(hadamard);
		} else {
			vector< shared_ptr<Qubit> > inputs(2);
			inputs.at(0) = Q.at(i-1);
			inputs.at(1) = Q.at(i);
			Q.at(i)->v->applyOperation(cnot, inputs);
			csc.SendQubit(Q.at(i));
			Q.at(i-1)->print();
		}
	}

	sleep(10);

	for ( i = 0; i < Q.size(); i+=2 ) {
		printf("Measured %i to be %i\r\n", i, Q.at(i)->measure());
		Q.at(i)->print();
	}
	
	printf("\r\n********** After Measurement **********\r\n\r\n");
	for ( i = 0; i < Q.size(); i++ ) {
		printf("Qubit at %i: \r\n", i);
		Q.at(i)->print();
	}
}
}
