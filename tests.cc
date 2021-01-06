#include "expression.h"

#include <iostream>
#include <exception>

int main() {
	try {
		nd_sm::expression e = "127 * cos( -3 + 3 )^2";
		std::cout << e << std::endl
			<< e.result() << std::endl;
	}
	catch (std::exception e) {
		std::cout << e.what();
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
