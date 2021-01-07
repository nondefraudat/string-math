#include "string_math.h"

#include <iostream>
#include <exception>

int main() {
	try {
		nd_sm::expression e = "ln10";
		std::cout << e << std::endl
			<< e.result() << std::endl;

		/*const size_t COUNT = 100000;
		size_t length = COUNT*2 + 1;
		char* test_e = new char[length + 1];
		test_e[0] = '1';
		for (size_t i = 1; i < length; i+=2) {
			test_e[i] = '+';
			test_e[i + 1] = '1';
		}
		test_e[length] = '\0';
		std::cout << nd_sm::calculate(test_e);*/
	}
	catch (std::exception e) {
		std::cout << e.what();
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
