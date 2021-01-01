#include "expression.h"

#include <iostream>
#include <exception>

int main() {
	try {
		nd_sm::expression test = "127*(3+4)";
		std::cout << test;
	}
	catch (std::exception e) {
		std::cout << e.what();
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
