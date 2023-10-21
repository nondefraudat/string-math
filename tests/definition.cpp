#include <string-math.hpp>

using namespace std;

int main() {
	Expression expression = "12 + 22 * 10";
	cout << expression;
	if (expression.getDefinition() != "12 + 22 * 10") {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
