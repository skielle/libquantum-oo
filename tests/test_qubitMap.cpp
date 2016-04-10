#include <fstream>
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include "qubitMap.h"
#include "stateVector.h"

using namespace std;
using namespace Quantum;

int main() {
	QubitMap* map = QubitMap::getInstance();

	shared_ptr<QubitMapEntry> qme = QubitMap::createQubit();

	qme->mapVector->print();

	return 0;
}
