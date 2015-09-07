/*
 * bb84Generation.cpp
 */

#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

#include "bb84Generation_runnable.h"
#include "system.h"

int main(int argc, char *argv[]) {
	int c;
	float et = 0;

	System* sys = System::getInstance();

	sys->setListenerPort(50084);
	
	BB84Generation_Runnable* bb84g = new BB84Generation_Runnable();

	while ( ( c = getopt(argc, argv, "b:k:e:t:" ) ) != EOF ) {
		switch ( c ) {
		case 'b':
			bb84g->numBases = stoi(optarg);
			break;
		case 'k':
			bb84g->keyLen = stoi(optarg);
			break;
		case 'e':
			bb84g->ecBits = stoi(optarg);
			break;
		case 't':
			et = stof(optarg);
			break;
		}
	}

	if ( et != 0 ) {
		if ( et < 1 ) {
			bb84g->ecTol = bb84g->ecBits * et;
		} else {
			bb84g->ecTol = et;
		}
	}

	shared_ptr<iRunnable> a ( bb84g );
	sys->setAlgorithm(a);

	sys->RunSystem();	

	return 0;
}
