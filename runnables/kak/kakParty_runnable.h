/*
 * kakParty_runnable.h
 */

#ifndef __KAK_PARTY_H
#define __KAK_PARTY_H

#include "iRunnable.h"

using namespace Quantum;
using namespace std;

class KAKParty_Runnable : public iRunnable {
	private:
		bool _isInstigator = false;
	public:
		bool isInstigator();
		void setInstigator(bool instigate);
		virtual void Run(); 
};

#endif
