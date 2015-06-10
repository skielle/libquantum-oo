/*
 * gates.cpp
 */
#include <stdio.h>
#include "complex.h"
#include "config.h"
#include "math.h"
#include "node.h"
#include "register.h"

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

		void run ( Register* r, int target) {
			Matrix m = Matrix(2, 2);

			m.set(0, 0, cos(psi / 2));
			m.set(1, 0, -IMAGINARY * sin(psi / 2));
			m.set(2, 0, -IMAGINARY * sin(psi / 2));
			m.set(3, 0, cos(psi / 2));

			r->applyMatrix(target, &m);
		}
};

class RotateY: public Gate {
	private:
		 float psi;

	public:
		RotateY () {
			this->psi = 0;
		}

		void setPsi ( float psi ) {
			this->psi = psi;
		}

		float getPsi () {
			return this->psi;
		}

		void run ( Register* r, int target) {
			Matrix m = Matrix(2, 2);

			m.set(0, 0, cos(psi / 2));
			m.set(1, 0, -1 * sin(psi / 2));
			m.set(2, 0, sin(psi / 2));
			m.set(3, 0, cos(psi / 2));

			r->applyMatrix(target, &m);
		}
};

class RotateZ: public Gate {
	private:
		 float psi;

	public:
		RotateZ () {
			this->psi = 0;
		}

		void setPsi ( float psi ) {
			this->psi = psi;
		}

		float getPsi () {
			return this->psi;
		}

		void run ( Register* r, int target) {
			Matrix m = Matrix(2, 2);

			m.set(0, 0, cos(psi / 2) - IMAGINARY * sin(psi / 2));
			m.set(1, 0, 0);
			m.set(2, 0, 0);
			m.set(3, 0, cos(psi / 2) + IMAGINARY * sin(psi / 2));

			r->applyMatrix(target, &m);
		}
};

class Hadamard: public Gate {
	public:
		void run ( Register* r, int target) {
			Matrix m = Matrix(2, 2);

			m.set(0, 0, sqrt(1.0/2));
			m.set(1, 0, sqrt(1.0/2));
			m.set(2, 0, sqrt(1.0/2));
			m.set(3, 0, -1 * sqrt(1.0/2));

			r->applyMatrix(target, &m);
		}
};

class SigmaX: public Gate {
	public:
		void run ( Register* r, int target) {
			Matrix m = Matrix(2, 2);

			m.set(0, 0, 0);
			m.set(1, 0, 1);
			m.set(2, 0, 1);
			m.set(3, 0, 0);

			r->applyMatrix(target, &m);
		}
};

class SigmaY: public Gate {
	public:
		void run ( Register* r, int target) {
			Matrix m = Matrix(2, 2);

			m.set(0, 0, 0);
			m.set(1, 0, -1 * IMAGINARY);
			m.set(2, 0, 1 * IMAGINARY);
			m.set(3, 0, 0);

			r->applyMatrix(target, &m);
		}
};
	
class SigmaZ: public Gate {
	public:
		void run ( Register* r, int target) {
			Matrix m = Matrix(2, 2);

			m.set(0, 0, 1);
			m.set(1, 0, 0);
			m.set(2, 0, 0);
			m.set(3, 0, -1);

			r->applyMatrix(target, &m);
		}
};

}

