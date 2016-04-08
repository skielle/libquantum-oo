/*
 * teleportClientRunnable.cpp
 */
//STD C++ includes
#include <memory>
#include <stdio.h>
#include <string>
#include <utility>
#include <unistd.h>
#include <vector>

//Qoosim includes
#include "channelService_client.h"
#include "gates.h"
#include "system.h"
#include "qubit.h"

//Runnable header
#include "teleportClientRunnable.h"

using namespace std;
namespace Quantum {
/*
 * Run
 * @param none
 * @returns none
 * @sides teleports a qubit to a remote peer
 */
void TeleportClientRunnable::Run() {
	//Create three qubits in initial state |0>
	shared_ptr<Qubit> q1 = Qubit::create();
	shared_ptr<Qubit> q2 = Qubit::create();
	shared_ptr<Qubit> psi = Qubit::create();
	
	//Create a hadamard gate
	Hadamard h;
	//Create a y-rotational gate
	Ry r(M_PI/3);
	//Create a Cnot gate
	CNot cn;
	
	//Get the system object
	System* sys = System::getInstance();
	//Create a connection to the server
	QuantumChannel::ChannelService_client csc(
		this->serverIP, 
		this->serverPort);

	//Rotate the payload qubit (this could be any operation)
	psi->applyMatrix(r);

	//Violate the laws of physics and print psi
	psi->print();

	//Apply a hadamard to q1, placing the qubit in a superposition
	q1->applyMatrix(h);

	//Entangle q1 and q2 by applying a Cnot gate
	vector< shared_ptr<Qubit> > inputs;
	inputs.push_back(q1);
	inputs.push_back(q2);
	q1->v->applyOperation(cn, inputs);

	//Send q2 to the server
	csc.SendQubit(q2);

	//Wait two seconds, for the server to get the qubit
	sleep(2);

	//Add the payload qubit to the entanglement by using Cnot
	inputs.at(0) = psi;
	inputs.at(1) = q1;
	q1->v->applyOperation(cn, inputs);

	//Apply a hadamard gate to the payload qubit
	psi->applyMatrix(h);

	//Measure q1 and psi
	int q1_result = q1->measure();
	int psi_result = psi->measure();
	printf("q1: %i, psi: %i\r\n", q1_result, psi_result);

	/*
	 * Normally, here, we would send q1_result and psi_result to the server
	 * so that she could perform the encoded post-teleportation operation
	 */

	sys->stopServer();
}
}
