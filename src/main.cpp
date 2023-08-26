#include "mathexpression.hpp"
#include <iostream>

using namespace std;

int main() {
	MathExpression expression = "2 + 2 * 2";
	cout << expression.definition() << '=' << expression.result() << endl;
	return EXIT_SUCCESS;
}
