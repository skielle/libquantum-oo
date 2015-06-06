/*
 * matrix.h
 */

#include "config.h"

#ifndef __QUANTUM_MATRIX_H
#define __QUQNTUM_MATRIX_H

namespace Quantum {
class Matrix {
	int rows;
	int cols;
	COMPLEX_FLOAT *t;

	long mem;
	long max;

	public:
		Matrix();
		Matrix(int cols, int rows);

		void setRows(int rows);
		void setCols(int cols);
		int getRows();
		int getCols();

		COMPLEX_FLOAT get(int x, int y);
		void set(int x, int y, COMPLEX_FLOAT v);
		static Matrix matrixMultiply(Matrix a, Matrix b);

		unsigned long memman(long change);

		void del();
		~Matrix();

		void print();
};
}
#endif
