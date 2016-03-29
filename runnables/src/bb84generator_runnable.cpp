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
	int i, runCounter;

	int runs = 20;

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

	for ( runCounter = 0; runCounter < runs; runCounter++ ) {
		keyMaterial.clear();
		int qubitsGenerated = 0;
		int qubitsLostToBaseMismatch = 0;
		int qubitsLostToErrorChecking = 0;
		int qubitsLostToReconciliation = 0;

	while( keyMaterial.size() < BB84_RAW_SIZE ) {
		for ( i = 0; i < BB84_BURST_SIZE; i++ ) {
			bases.at(i) = round(rand() / (float)RAND_MAX);
			bits.at(i) = round(rand() / (float)RAND_MAX);

			Q.at(i) = Qubit::create();
			qubitsGenerated++;

			if ( bits.at(i) == 1 ) {
				Q.at(i)->applyMatrix(gSigmaX);
			}

			if ( bases.at(i) == 1 ) {
				Q.at(i)->applyMatrix(gHadamard);
			}
			csc.SendQubit(Q.at(i));
		}

		while ( sys->isMessageQueueEmpty() ) { 
		}

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
					qubitsLostToBaseMismatch++;
					validBases += "0";
				}
			}
			csc.SendClassicData(validBases);
		}
	}

	while ( sys->isMessageQueueEmpty() ) {
	}
	if ( sys->nextMessageType() == SystemMessage::CLASSIC_DATA_RECEIVED ) {
		int rawMaterialLen = keyMaterial.size();
		int address = sys->nextMessage();
		int numErrors = 0;
		string checkBits = sys->getClassicData(address);
		int numCheckBits = checkBits.length();

		for ( i = 0; i < checkBits.length(); i++ ) {
			if ( checkBits.at(i) - '0' != 
				keyMaterial.at(
					rawMaterialLen - numCheckBits + i) ) {
				numErrors++;
			}
		}
		qubitsLostToErrorChecking = numCheckBits;
		qubitsLostToReconciliation = 
			((float)numErrors / (float)numCheckBits ) * 
			(float)(rawMaterialLen - numCheckBits) * 2;

		printf("%i,%i,%i,%i,%i\r\n", qubitsGenerated, 
			qubitsLostToBaseMismatch,
			qubitsLostToErrorChecking, numErrors, 
			qubitsLostToReconciliation );
	}
	}
}
}
