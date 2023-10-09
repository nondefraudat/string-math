#include "parser.hpp"
#include <iostream>

using namespace std;

int main() {
	std::string expression("2 + 2 * 2");
	Parser parser;
	NodePtr root = parser.parse(expression);
	if (!root) {
		cout << "error\n";
	}
	else {
		cout << root->getDefinition() << " = " << root->getResult() << endl;
	}
	return EXIT_SUCCESS;
}
