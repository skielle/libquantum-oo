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

	string basesChosen;
	string validBases = "";
	vector<int> bases(BB84_BURST_SIZE);
	vector<int> bits(BB84_BURST_SIZE);
	vector<int> keyMaterial;

	vector< shared_ptr<Qubit> > Q(BB84_BURST_SIZE);

	srand(time(NULL) / getpid());

	SigmaX gSigmaX;
	Hadamard gHadamard;
	
	System* sys = System::getInstance();
	QuantumChannel::ChannelService_client csc(this->serverIP, 
		this->serverPort);

	while( keyMaterial.size() < BB84_RAW_SIZE ) {
		sleep(3);
		printf("Sending qubits...\r\n");
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
		}

		printf("Waiting for base message...\r\n");
		while ( sys->isMessageQueueEmpty() ) { 
			sleep(1);
		}

		printf("Processing base message...\r\n");
		if ( sys->nextMessageType() 
			== SystemMessage::CLASSIC_DATA_RECEIVED ) {	
			int address = sys->nextMessage();
			basesChosen = sys->getClassicData(address);

			validBases = "";
			for ( i = 0; i < basesChosen.length(); i++ ) {
				if ( basesChosen.at(i) - '0' == bases.at(i) ) {
					keyMaterial.push_back(bits.at(i));
					validBases += "1";
				} else {
					validBases += "0";
				}
			}
			printf("Sending matching bases...\r\n");
			csc.SendClassicData(validBases);
		}
		printf("Key Material: %i\r\n", keyMaterial.size());
	}
}
}
