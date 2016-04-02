/*
 * matrix.h
 */
#include <complex>
#include <string.h>
#include <vector>
//#include "config.h"
#include "quantumMessage.pb.h"

#ifndef __QUANTUM_MATRIX_H
#define __QUANTUM_MATRIX_H

using namespace std;

namespace Quantum {
class Matrix {
	int rows;
	int cols;
	vector< complex<double> > *t;

	public:
		Matrix();
		Matrix(int cols, int rows);
		Matrix(const Matrix &m);

		void setRows(int rows);
		void setCols(int cols);
		int getRows();
		int getCols();

		static complex<double> determinant(Matrix m);
		static Matrix inverse(Matrix m);
		complex<double> get(int x, int y);
		void set(int x, int y, complex<double> v);
		void copy(const Matrix &m);
		static Matrix matrixTensor(Matrix a, Matrix b);
		static Matrix matrixMultiply(Matrix a, Matrix b);
		static Matrix identity();

		~Matrix();

		void print();

		QuantumMessage::MatrixMessage serialize();
		static Matrix unserialize(
			const QuantumMessage::MatrixMessage* loadMessage);
};
}
#endif
