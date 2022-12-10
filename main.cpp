#include <iostream>

#include "Type.h"

#include <memory>


int main()
{
	Int var = 1;

	Int var2 = 2;

	var2 = var;
	var = 123;

	std::cout << *var << "\n";
	std::cout << *var2 << "\n";

	return 0;
}
