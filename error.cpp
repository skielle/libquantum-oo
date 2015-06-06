/*
 * error.cpp 
 */
#include <stdlib.h>
#include <stdio.h>
#include <exception>
#include <stdexcept>
#include "error.h"

using namespace std;

namespace Quantum {
//depricated
void Error::errorHandler(void *f(int)) {
	return;
}
 
const char *Error::strerr(int errno) {
	switch(errno) {
		case QUANTUM_SUCCESS:
			return "success";
		case QUANTUM_FAILURE:
			return "failure";
		case QUANTUM_ENOMEM:
			return "malloc failed";
		case QUANTUM_EMLARGE:
			return "matrix too large";
		case QUANTUM_EMSIZE:
			return "wrong matrix size";
		case QUANTUM_EHASHFULL:
			return "hash table full";
		case QUANTUM_EMCMATRIX:
			return "single-column matrix expected";
		case QUANTUM_EOPCODE:
			return "unknown opcode";
		default:
			return "unknown error code";
	}
}
		
void Error::error(int errno) {
	throw runtime_error(Error::strerr(errno));
}
}
