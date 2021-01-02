#include "expression.h"

#include <iostream>
#include <exception>

int main() {
	try {
		nd_sm::expression e = "--127 * ( -3 + 4 )";
		e.test();
	}
	catch (std::exception e) {
		std::cout << e.what();
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
