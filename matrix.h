/*
 * matrix.h
 */
#include <string.h>
#include <vector>
#include "config.h"
#include "quantumMessage.pb.h"

#ifndef __QUANTUM_MATRIX_H
#define __QUANTUM_MATRIX_H

using namespace std;

namespace Quantum {
class Matrix {
	int rows;
	int cols;
	vector<COMPLEX_FLOAT> *t;

	public:
		Matrix();
		Matrix(int cols, int rows);

		void setRows(int rows);
		void setCols(int cols);
		int getRows();
		int getCols();

		static COMPLEX_FLOAT determinant(Matrix m);
		static Matrix inverse(Matrix m);
		COMPLEX_FLOAT get(int x, int y);
		void set(int x, int y, COMPLEX_FLOAT v);
		static Matrix matrixMultiply(Matrix a, Matrix b);

		~Matrix();

		void print();

		QuantumMessage::MatrixMessage serialize();
		static Matrix unserialize(
			const QuantumMessage::MatrixMessage* loadMessage);
};
}
#endif
