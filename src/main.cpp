#include "expression.hpp"
#include <iostream>

using namespace std;

int main() {
	Expression expression = "119 + 20 * 10";
	cout << expression << " = " << expression.getResult();
	return EXIT_SUCCESS;
}
