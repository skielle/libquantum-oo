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
	int i;

	vector<int> bases(BB84_BURST_SIZE);
	vector<int> bits(BB84_BURST_SIZE);

	srand(time(NULL) / getpid());

	Hadamard gHadamard;
	QubitMap* qm = QubitMap::getInstance();

	//wait for signal
	sleep(10);

	for ( i = 0; i < qm->numQubits(); i++ ) {
		shared_ptr<Qubit> q = qm->getQubit(i);
		bases.at(i) = round(rand() / (float)RAND_MAX);

		if ( bases.at(i) == 1 ) {
			q->applyMatrix(gHadamard);
		}
		bits.at(i) = q->measure();

		if ( bases.at(i) == 0 ) {
			printf("+%i\r\n", bits.at(i));
		} else {
			printf("x%i\r\n", bits.at(i));
		}
	}
}
}
