#include "expression.hpp"
#include <limits>

using namespace std;

std::ostream& operator<<(std::ostream& os, const Expression& expression) {
	return os << expression.parseDefinition();
}

Expression::Expression(const std::string& expression, Parser parser) noexcept
	: root(parser.parse(expression)) { }

Expression::Expression(const char* expression) noexcept
	: Expression(expression, Parser()){ }

bool Expression::isValid() const noexcept {
	return root != nullptr;
}

std::string Expression::parseDefinition() const noexcept {
	if (!isValid()) {
		return "error parsing";
	}
	return root->parseDefinition();
}

double Expression::calculate() const noexcept {
	if (!isValid()) {
		return numeric_limits<double>::quiet_NaN();
	}
	return root->calculate();
}
