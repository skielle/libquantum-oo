/*
 * gates.cpp
 */
#include <stdio.h>
#include "complex.h"
#include "config.h"
#include "gate.h"
#include "node.h"

namespace Quantum {
class RotateX: public Gate {
	private:
		 float psi;

	public:
		RotateX () {
			this->psi = 0;
		}

		void setPsi ( float psi ) {
			this->psi = psi;
		}

		float getPsi () {
			return this->psi;
		}

		void run ( Node* target) {
			printf("Got target %f\n", Complex::real(target->getAmplitude()));
		}
};
}
