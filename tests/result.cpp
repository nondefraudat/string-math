#include <string-math.hpp>

int main() {
	if (calculate("2 + 2 * 2") != 6) {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
