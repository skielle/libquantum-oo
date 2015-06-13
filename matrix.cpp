/*
 * matrix.cpp
 */

#include <stdio.h>
#include <stdlib.h>
#include "complex.h"
#include "config.h"
#include "error.h"
#include "matrix.h"
#include "system.h"

using namespace std;

namespace Quantum {
Matrix::Matrix(int cols, int rows) {
	this->setRows(rows);
	this->setCols(cols);

	this->t = new COMPLEX_FLOAT [ this->getRows() * this->getCols() ];

	if ( !this->t ) {
		Error::error(QUANTUM_ENOMEM);
	}

	System::memman(this->getRows() * this->getCols() * sizeof(COMPLEX_FLOAT));	
}

void Matrix::setRows(int rows) { this->rows = rows; }
void Matrix::setCols(int cols) { this->cols = cols; }
int Matrix::getRows() { return this->rows; }
int Matrix::getCols() { return this->cols; }

COMPLEX_FLOAT Matrix::get( int x, int y ) {
	return this->t[ x + y * this->getCols() ];
}

void Matrix::set( int x, int y, COMPLEX_FLOAT v ) {
	this->t[ x + y * this->getCols() ] = v;
}

Matrix Matrix::matrixMultiply(Matrix a, Matrix b) {
	int i, j, k;
	COMPLEX_FLOAT cVal;

	if ( a.getCols() != b.getRows() ) {
		Error::error(QUANTUM_EMSIZE);
	}

	Matrix c = Matrix( b.getCols(), a.getRows() );

	for ( i = 0; i < b.getCols(); i++ ) {
		for ( j = 0; j < a.getRows(); j++ ) {
			cVal = 0;
			for ( k = 0; k < b.getRows(); k++ ) {
				cVal += a.get( k, j ) * b.get( i, k );
			}
			c.set( i, j, cVal );
		}
	}
	return c;
}

void Matrix::del() { this->~Matrix(); }
Matrix::~Matrix() {
	//free(this->t);
	System::memman(-sizeof(COMPLEX_FLOAT) * this->getCols() * this->getRows());
	this->t = 0;
}

void Matrix::print() {
	int i, j;
	int z = 0;

	while ( ( 1 << z++ ) < this->getRows() );
	z--;

	for ( i = 0; i < this->getRows(); i++ ) {
		for ( j = 0; j < this->getCols(); j++ ) {
			printf("%g %+gi", Complex::real( this->get( j, i ) ),
				Complex::imaginary( this->get( j, i ) ) );
		}
		printf("\n");
	}
	printf("\n");
}
}
