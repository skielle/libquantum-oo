/*
 * amplitudeDamping_dual.cpp
 */
#include <memory>
#include <unistd.h>
#include <stdio.h>

#include "amplitudeDamping_dual.h"
#include "matrix.h"
#include "qubit.h"
#include "hadamard.h"

using namespace std;

namespace Quantum {
void AmplitudeDamping_dual::doEvil(shared_ptr<Qubit> q) {
	Matrix E_AD(2,2);
	Matrix CNOT(4,4);
	Hadamard h;

	shared_ptr<Qubit> p = Qubit::create();

	E_AD.set(0, 0, 1);
	E_AD.set(0, 1, sqrt(this->eta));
	E_AD.set(1, 0, sqrt(this->eta));
	E_AD.set(1, 1, sqrt(1-this->eta));

	CNOT.set(0, 0, 1);
	CNOT.set(1, 1, 1);
	CNOT.set(2, 3, 1);
	CNOT.set(3, 2, 1);

	vector< shared_ptr<Qubit> > inputs;
	inputs.push_back(q);
	inputs.push_back(p);

	q->v->applyOperation(CNOT, inputs);
	p->applyMatrix(h);
	inputs.at(0)=p;
	inputs.at(1)=q;
	q->v->applyOperation(CNOT, inputs);	

	q->applyMatrix(E_AD);

	q->v->applyOperation(CNOT, inputs);
	p->applyMatrix(h);
	inputs.at(0)=q;
	inputs.at(1)=p;
	q->v->applyOperation(CNOT, inputs);
}

void AmplitudeDamping_dual::setEta(float newEta) {
	this->eta = newEta;
}

float AmplitudeDamping_dual::getEta() {
	return this->eta;
}

}
