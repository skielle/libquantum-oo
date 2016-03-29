/*
 * kakinitiator_runnable.cpp
 */
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <string>
#include <vector>

#include "system.h"
#include "qubit.h"
#include "qubitMap.h"
#include "gates.h"
#include "channelService_client.h"
#include "kakinitiator_runnable.h" 
#include "kakresponder_runnable.h"

using namespace std;

namespace Quantum {
void KakInitiator_Runnable::Run() {
	int i;

	int errors = 0;
	vector<int> bits;
	vector<float> rotations;

	srand(time(NULL) / getpid());

	System* sys = System::getInstance();
	QubitMap* qm = QubitMap::getInstance();
	QuantumChannel::ChannelService_client csc(this->serverIP, 
		this->serverPort);
	vector< shared_ptr<Qubit> > q;
	SigmaX sx;

	for ( i = 0; i < KAK_KEY_SIZE; i++ ) {
		if ( i % KAK_BURST_SIZE == 0 ) { sleep(1); }
		bits.push_back(round(rand() / (float)RAND_MAX));
		rotations.push_back((rand() / (float)RAND_MAX) * M_PI/8.0);

		q.push_back(Qubit::create());
		if ( bits.at(i) == 1 ) {
			q.at(i)->applyMatrix(sx);
		}


		Ry r = Ry(rotations.at(i));
		q.at(i)->applyMatrix(r);

		csc.SendQubit(q.at(i));
	}

	for ( i = 0; i < KAK_KEY_SIZE; i++ ) {
		if ( i % KAK_BURST_SIZE == 0 ) { sleep(1); }
		while ( sys->isMessageQueueEmpty() ) { 
		}
		//receive qubit
		int address = sys->nextMessage();
		q.at(i) = qm->getQubit(address);	
	}

	for ( i = 0; i < KAK_KEY_SIZE; i++ ) {
		Ry r = Ry(-1.0*rotations.at(i));
		q.at(i)->applyMatrix(r);
	}
	for ( i = 0; i < KAK_KEY_SIZE; i++ ) {
		if ( i % KAK_BURST_SIZE == 0 ) { sleep(1); }
		csc.SendQubit(q.at(i));
	}

	for ( i = 0; i < KAK_KEY_SIZE; i++ ) {
		if ( i % KAK_BURST_SIZE == 0 ) { sleep(1); }
		while ( sys->isMessageQueueEmpty() ) {
		}

		int address = sys->nextMessage();
		string data = sys->getClassicData(address);
		if ( data == to_string(bits.at(i)) ) {
			errors++;
		}
	}

	printf("%i, %i\r\n", KAK_KEY_SIZE, errors);
	sys->stopServer();
}
}
