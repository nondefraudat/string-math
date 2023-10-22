#include "tests.hpp"
#include <string-math.hpp>

using namespace std;

Tests::Tests() noexcept : status(true) { }

void Tests::add(const std::string& source, const std::string& target, const double result) noexcept {
	Expression expression(source);
	if (expression.parseDefinition() != target) {
		cout << "Definition parsing error!\nSource:\t\t\"" << source <<
				"\"\nExpected:\t\"" << target <<
				"\"\nGot:\t\t\"" << expression.parseDefinition() << "\"\n";
		status = false;
	}
	if (expression.calculate() != result) {
		cout << "Calculating error!\nSource: \"" << source <<
				"\"\nExpected value \"" << result << ", but got \"" << expression.calculate() << "\"\n";
		status = false;
	}
}

int Tests::getStatus() const noexcept {
	return status ? EXIT_SUCCESS : EXIT_FAILURE;
}
