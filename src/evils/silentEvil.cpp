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
