/*
 * bb84generator_runnable.cpp
 */
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <string>
#include <vector>

#include "system.h"
#include "qubit.h"
#include "gates.h"
#include "channelService_client.h"
#include "bb84generator_runnable.h" 

using namespace std;

namespace Quantum {
void BB84Generator_Runnable::Run() {
	int i;

	vector<int>bases(BB84_BURST_SIZE);
	vector<int>bits(BB84_BURST_SIZE);

	vector< shared_ptr<Qubit> > Q(BB84_BURST_SIZE);

	srand(time(NULL) / getpid());

	SigmaX gSigmaX;
	Hadamard gHadamard;
	
	QuantumChannel::ChannelService_client csc(this->serverIP, this->serverPort);

	for ( i = 0; i < BB84_BURST_SIZE; i++ ) {
		bases.at(i) = round(rand() / (float)RAND_MAX);
		bits.at(i) = round(rand() / (float)RAND_MAX);

		Q.at(i) = Qubit::create();

		if ( bits.at(i) == 1 ) {
			Q.at(i)->applyMatrix(gSigmaX);
		}

		if ( bases.at(i) == 1 ) {
			Q.at(i)->applyMatrix(gHadamard);
		}
		csc.SendQubit(Q.at(i));

		if ( bases.at(i) == 0 ) {
			printf("+%i\r\n", bits.at(i));
		} else {
			printf("x%i\r\n", bits.at(i));
		}
	}
}
}
