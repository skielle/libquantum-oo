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
	//to store the client's ip/port once they connect
	string peerIP = "";
	int peerServicePort = 0;

	//vector for storing the bits we decode
	vector<int> keyMaterial;
	//vector for storing rotation bits we use to encode
	vector<float> rotations;
	//vector for storing qubits (scratch space)
	vector< shared_ptr<Qubit> > q;

	//seed random, unique to this process
	srand(time(NULL) / getpid());

	//get a reference to the system and qubit map
	System* sys = System::getInstance();
	QubitMap* qm = QubitMap::getInstance();
	//declared for client callback connection
	QuantumChannel::ChannelService_client* csc;

	//receive qubits rotated by our peer
	for ( i = 0; i < KAK_KEY_SIZE; i++ ) {
		//generate a random rotation value 0 < rotation <= 1
		rotations.push_back((rand() / (float)RAND_MAX) * M_PI / 8.0);
		//wait for the system to receive the qubit and put
		//a message on the queue
		while ( sys->isMessageQueueEmpty() ) {
		}
		//get the map address of the received qubit
		int address = sys->nextMessage();
		//store the received qubit in the scratch vector
		q.push_back(qm->getQubit(address));

		//if we haven't yet established a callback connection to the
		//client, do so now
		if ( peerIP == "" ) {
			peerIP = q.at(i)->origin.peerIP;
			peerServicePort = q.at(i)->origin.peerServicePort;
			csc = new QuantumChannel::ChannelService_client(
					peerIP, peerServicePort);
		}
	}

	//rotate the returned qubits and send back
	for ( i = 0; i < KAK_KEY_SIZE; i++ ) {
		//create a rotational gate
		Ry r = Ry(rotations.at(i));
		//apply the rotational gate
		q.at(i)->applyMatrix(r);
		//send the qubit
		csc->SendQubit(q.at(i));
	}

	//receive qubits which our peer has counter-rotated
	for ( i = 0; i < KAK_KEY_SIZE; i++ ) {
		//wait for the system to receive the qubit and put
		//a message on the queue
		while ( sys->isMessageQueueEmpty() ) {
		}
		//get the map address of the received qubit
		int address = sys->nextMessage();
		//store the received qubit in the scratch vector
		q.at(i) = qm->getQubit(address);
	}

	//counter-rotate the returned qubits and measure
	for ( i = 0; i < KAK_KEY_SIZE; i++ ) {
		//create the counter-rotational gate for this qubit
		Ry r = Ry(-1 * rotations.at(i));
		//apply the counter-rotational gate
		q.at(i)->applyMatrix(r);
		//measure the qubit
		string data = to_string(q.at(i)->measure());
		
		//this part is outside of Kak's protocol
		//we're going to send the results to our peer
		//and they'll count how many errors were introduced
		csc->SendClassicData(data);
	}

	//stop the server thread
	sys->stopServer();
}
}
