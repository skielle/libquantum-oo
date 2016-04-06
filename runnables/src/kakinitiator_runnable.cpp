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
	//count of errors (bits mismatched)
	int errors = 0;
	//vector for storing bits we sent
	vector<int> bits;
	//vector for storing rotational angles used to encode bits
	vector<float> rotations;
		
	//vector for storing qubits (scratch space)	
	vector< shared_ptr<Qubit> > q;
	//quantum Sigma X gate
	SigmaX sx;
	
	//seed random, unique to this process
	srand(time(NULL) / getpid());

	//get a reference to the system and qubit map
	System* sys = System::getInstance();
	QubitMap* qm = QubitMap::getInstance();
	
	//set up connection to the server
	QuantumChannel::ChannelService_client csc(this->serverIP, 
		this->serverPort);

	//this implementation sends a key of fixed size, with all
	//key-bits sent in serial
	for ( i = 0; i < KAK_KEY_SIZE; i++ ) {
		//introduce a small delay to prevent resource exhaustion
		//on localhost <----> localhost connections
		if ( i % KAK_BURST_SIZE == 0 ) { sleep(1); }
		//generate a random bit value {0, 1}
		bits.push_back(round(rand() / (float)RAND_MAX));
		//generate a random rotation value 0 < rotation <= 1
		rotations.push_back((rand() / (float)RAND_MAX) * M_PI/8.0);

		//create a new qubit (initial value |0>)
		q.push_back(Qubit::create());
		//if the bit is to be a 1, apply a sigma-x gate
		if ( bits.at(i) == 1 ) {
			q.at(i)->applyMatrix(sx);
		}

		//create a rotational gate
		Ry r = Ry(rotations.at(i));
		//apply the rotational gate
		q.at(i)->applyMatrix(r);

		//send the qubit
		csc.SendQubit(q.at(i));
	}

	//receive returned qubits
	for ( i = 0; i < KAK_KEY_SIZE; i++ ) {
		if ( i % KAK_BURST_SIZE == 0 ) { sleep(1); }
		//wait for the system to receive the qubit and put
		//a message on the queue
		while ( sys->isMessageQueueEmpty() ) { 
		}
		//get the map address of the received qubit
		int address = sys->nextMessage();
		//store the received qubit in the scratch vector
		q.at(i) = qm->getQubit(address);	
	}

	//counter-rotate the returned qubits and resend
	for ( i = 0; i < KAK_KEY_SIZE; i++ ) {
		//create the counter-rotational gate for this qubit
		Ry r = Ry(-1.0*rotations.at(i));
		//apply the counter-rotational gate
		q.at(i)->applyMatrix(r);
	}
	for ( i = 0; i < KAK_KEY_SIZE; i++ ) {
		if ( i % KAK_BURST_SIZE == 0 ) { sleep(1); }
		//return the qubit
		csc.SendQubit(q.at(i));
	}

	//this part is outside of Kak's protocol
	//we're going to receive the results from our peer
	//and count how many errors were introduced
	for ( i = 0; i < KAK_KEY_SIZE; i++ ) {
		if ( i % KAK_BURST_SIZE == 0 ) { sleep(1); }
		//wait for a message on the queue
		while ( sys->isMessageQueueEmpty() ) {
		}

		//get the map address of the received bit
		int address = sys->nextMessage();
		//return the data stored at the given address
		string data = sys->getClassicData(address);
		//if the data received doesn't match our original
		//bit vector, count it as an error
		if ( data == to_string(bits.at(i)) ) {
			errors++;
		}
	}

	//output error statistics
	printf("%i, %i\r\n", KAK_KEY_SIZE, errors);
	//stop the server thread
	sys->stopServer();
}
}
