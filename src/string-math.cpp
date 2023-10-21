#include "string-math.hpp"

double calculate(const std::string& sourceExpression) noexcept {
	Expression expression(sourceExpression);
	return expression.calculate();
}
