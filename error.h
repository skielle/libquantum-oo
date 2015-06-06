/*
 * complex.h
 */

#ifndef __QUANTUM_ERROR_H
#define __QUANTUM_ERROR_H

namespace Quantum {
class Error {
	public:
		enum {
			QUANTUM_SUCCESS   = 0,
			QUANTUM_FAILURE   = 1,
			QUANTUM_ENOMEM    = 2,
			QUANTUM_EMLARGE   = 3,
			QUANTUM_EMSIZE    = 4,
			QUANTUM_EHASHFULL = 5,
			QUANTUM_EMCMATRIX = 65536,
			/* internal errors start at 65536 */
			QUANTUM_EOPCODE   = 65537
		};
	
		static void errorHandler(void *f(int));
		static const char *strerr(int errno);
		static void error(int errno);
};
}
#endif
