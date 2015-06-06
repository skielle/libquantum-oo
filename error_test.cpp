#include <stdio.h>
#include <math.h>
#include <exception>
#include <stdexcept>
#include "error.h"
using namespace std;
using namespace Quantum;

int main() {
	try {
		Error::error(0);
	} catch (exception& e) {
		printf("%s\n", e.what());
	}
	return 0;
}
