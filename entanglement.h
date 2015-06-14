/*
 * entanglement.h
 */

#include "register.h"
#include "entangledRegister.h"

namespace Quantum {
class Entanglement {
	protected:
		EntangledRegister* alpha;
		EntangledRegister* beta;
		EntangledPair* entanglements;
	public:
		static Entanglement createEntanglement();
		EntangledRegister getAlpha();
		EntangledRegister getBeta();
		void entangle(int target, EntanglementPair entanglementData);

		bool isEntangled( int target);

		void applyMatrix(int target, Matrix* m);

		viod measure(int target);
};
}		
