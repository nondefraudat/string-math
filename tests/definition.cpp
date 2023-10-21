#include <string-math.hpp>

int main() {
	Expression expression = "2 + 2 * 2";
	if (expression.parseDefinition() != "2 + 2 * 2") {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
