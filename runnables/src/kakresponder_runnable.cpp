/*
 * kakresponder_runnable.cpp
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
void KakResponder_Runnable::Run() {
	int i;
	string peerIP = "";
	int peerServicePort = 0;

	vector<int> keyMaterial;
	vector<float> rotations;

	srand(time(NULL) / getpid());

	System* sys = System::getInstance();
	QuantumChannel::ChannelService_client* csc;
	QubitMap* qm = QubitMap::getInstance();
	vector< shared_ptr<Qubit> > q;

	for ( i = 0; i < KAK_KEY_SIZE; i++ ) {
		rotations.push_back((rand() / (float)RAND_MAX) * M_PI / 8.0);
		while ( sys->isMessageQueueEmpty() ) {
		}
		int address = sys->nextMessage();
		q.push_back(qm->getQubit(address));

		if ( peerIP == "" ) {
			peerIP = q.at(i)->origin.peerIP;
			peerServicePort = q.at(i)->origin.peerServicePort;
			csc = new QuantumChannel::ChannelService_client(
					peerIP, peerServicePort);
		}
	}

	for ( i = 0; i < KAK_KEY_SIZE; i++ ) {
		Ry r = Ry(rotations.at(i));
		q.at(i)->applyMatrix(r);
		csc->SendQubit(q.at(i));
	}

	for ( i = 0; i < KAK_KEY_SIZE; i++ ) {
		while ( sys->isMessageQueueEmpty() ) {
		}
		int address = sys->nextMessage();
		q.at(i) = qm->getQubit(address);
	}

	for ( i = 0; i < KAK_KEY_SIZE; i++ ) {
		//counter-rotate q
		Ry r = Ry(-1 * rotations.at(i));
		q.at(i)->applyMatrix(r);
		string data = to_string(q.at(i)->measure());
		csc->SendClassicData(data);
	}

	sys->stopServer();
}
}
