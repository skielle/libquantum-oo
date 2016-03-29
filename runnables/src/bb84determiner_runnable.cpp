/*
 * bb84determiner_runnable.cpp
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
#include "bb84determiner_runnable.h"
#include "bb84generator_runnable.h"

using namespace std;

namespace Quantum {
void BB84Determiner_Runnable::Run() {
	int i, runCounter;

	int runs = 20;
	int numCheckBits = (BB84_RAW_SIZE - BB84_KEY_SIZE ) / 2;

	vector<int> bases(BB84_BURST_SIZE);
	vector<int> bits(BB84_BURST_SIZE);
	vector<int> keyMaterial;

	srand(time(NULL) / getpid());

	Hadamard gHadamard;

	System* sys = System::getInstance();
	QubitMap* qm = QubitMap::getInstance();
	QuantumChannel::ChannelService_client* csc;

	string peerIP = "";
	int peerServicePort = 0;

	string validBases;

	for ( runCounter = 0; runCounter < runs; runCounter++ ) {
		keyMaterial.clear();
	while ( keyMaterial.size() < BB84_RAW_SIZE ) {
		for ( i = 0; i < bits.size(); i ++ ) {
			while ( sys->isMessageQueueEmpty() ) {
			}

			if ( sys->nextMessageType() 
				== SystemMessage::QUANTUM_DATA_RECEIVED ) {
				int address = sys->nextMessage();
				
				shared_ptr<Qubit> q = qm->getQubit(address);
				if ( peerIP == "" ) {
					peerIP = q->origin.peerIP;
					peerServicePort = 
						q->origin.peerServicePort;
					csc = new QuantumChannel::
						ChannelService_client(
						peerIP, peerServicePort);
				}

				if ( q->origin.peerIP == peerIP ) {
					bases.at(i) = 
						round(rand() / (float)RAND_MAX);
					if ( bases.at(i) == 1 ) {
						q->applyMatrix(gHadamard);
					}
					bits.at(i) = q->measure();
				}
			} else {
				sys->nextMessage();
			}
		}

		string basesChosen;
		for ( i = 0; i < bases.size(); i++ ) {
			basesChosen += to_string(bases.at(i));
		}
		csc->SendClassicData(basesChosen);

		while ( sys->isMessageQueueEmpty() ) {
		}

		if ( !( sys->nextMessageType() 
			== SystemMessage::CLASSIC_DATA_RECEIVED) ) {
		}

		if ( sys->nextMessageType() 
			== SystemMessage::CLASSIC_DATA_RECEIVED) {
			int address = sys->nextMessage();
				
			validBases = sys->getClassicData(address);
		}

		for ( i = 0; i < validBases.length(); i++ ) {
			if ( validBases.at(i) - '0' == 1 ) {
				keyMaterial.push_back(bits.at(i));
			}
		}
		printf("Key Material: %i\r\n", keyMaterial.size());
	}
	string checkBits = "";
	int rawMaterialLen = keyMaterial.size();
	for ( i = rawMaterialLen - numCheckBits; i < rawMaterialLen; i++ ) {
		checkBits += to_string(keyMaterial.at(i));
	}
	csc->SendClassicData(checkBits);

	}
}
}
