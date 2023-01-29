#include <iostream>

#include "Initialization.h"
#include "Definition.h"

MCRCON::GlobalVariable globalVariable;
MCRCON::Initialization init(globalVariable);

int main()
{
	init.init();
	return 0;
}
