/*
 * amplitudeDamping.cpp
 */
#include <memory>
#include <unistd.h>
#include <stdio.h>

#include "amplitudeDamping.h"
#include "matrix.h"
#include "qubit.h"

using namespace std;

namespace Quantum {
void AmplitudeDamping::doEvil(shared_ptr<Qubit> q) {
	Matrix E_AD(2,2);

	E_AD.set(0, 0, 1);
	E_AD.set(0, 1, sqrt(this->eta));
	E_AD.set(1, 0, sqrt(this->eta));
	E_AD.set(1, 1, sqrt(1-this->eta));

	q->applyMatrix(E_AD);
}

void AmplitudeDamping::setEta(float newEta) {
	this->eta = newEta;
}

float AmplitudeDamping::getEta() {
	return this->eta;
}

}
