/*
 * echoRunnable.cpp
 */
#include <memory>
#include <unistd.h>
#include <stdio.h>

#include "silentEvil.h"
#include "qubit.h"

using namespace std;

namespace Quantum {
void SilentEvil::doEvil(shared_ptr<Qubit> q) {
	Matrix E_AD(2,2);

	E_AD.set(0, 0, 1);
	E_AD.set(0, 1, sqrt(.1));
	E_AD.set(1, 0, sqrt(.1));
	E_AD.set(1, 1, sqrt(1-.1));

	q->applyMatrix(E_AD);


	float evilFactor = rand() / (float)RAND_MAX;
	if ( evilFactor <= evilness ) {
		q->measure();
	}
}

void SilentEvil::setEvilness(float newEvilness) {
	this->evilness = newEvilness;
}

float SilentEvil::getEvilness() {
	return this->evilness;
}

}
