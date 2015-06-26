/*
 * matrix.cpp
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "complex.h"
#include "config.h"
#include "error.h"
#include "matrix.h"
#include "quantumMessage.pb.h"
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

COMPLEX_FLOAT Matrix::determinant(Matrix m) {
	if ( m.getCols() != m.getRows() ) {
		Error::error(QUANTUM_EINVMATRIX);
	}

	if ( m.getCols() == 2 ) {
		return m.get(0, 0) * m.get(1, 1) 
			- m.get(0, 1) * m.get(1, 0);
	} else {
		//recurse here
	}		
}

Matrix Matrix::inverse(Matrix m) {
	COMPLEX_FLOAT detr = Matrix::determinant(m);
	if ( Complex::probability(detr) > .001 ) { 
//		Error::error(QUANTUM_EINVMATRIX);
	}
	Matrix inv = Matrix(m.getRows(), m.getCols());
	if ( m.getCols() == 2 ) {
		inv.set(0, 0, m.get(1, 1) / detr );
		inv.set(1, 1, m.get(0, 0) / detr );
		inv.set(0, 1, m.get(0, 1) * -1 / detr );
		inv.set(1, 0, m.get(1, 0) * -1 / detr );
		return inv;
	}

	if ( m.getCols() == 3 ) {
	}

	if ( m.getCols() == 4 ) {
	}

	Error::error(QUANTUM_EINVMATRIX);
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

void Matrix::serialize(iostream &output) {
	int i;
	QuantumMessage::MatrixMessage saveMessage;
	
	saveMessage.set_rows(this->getRows());
	saveMessage.set_cols(this->getCols());

	for ( i = 0; i < this->getRows() * this->getCols(); i++ ) {
		QuantumMessage::ComplexMessage* saveT = saveMessage.add_t();

		saveT->set_real(Complex::real(this->t[i]));
		saveT->set_imaginary(Complex::imaginary(this->t[i]));
	}

	saveMessage.SerializeToOstream(&output);
}

Matrix Matrix::unserialize(iostream &input) {
	int i;
	QuantumMessage::MatrixMessage loadMessage;
	loadMessage.ParseFromIstream(&input);

	Matrix m ( loadMessage.cols(), loadMessage.rows() );

	for ( i = 0; i < loadMessage.t_size(); i++ ) {
		QuantumMessage::ComplexMessage cMessage = loadMessage.t(i);

		m.set(i % m.getCols(), i / m.getCols(), 
			 cMessage.real() + cMessage.imaginary() * IMAGINARY);
	}

	return m;
}
}
