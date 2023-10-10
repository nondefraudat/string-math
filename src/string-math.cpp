#include "string-math.hpp"

double calculate(std::string& sourceExpression) noexcept {
	Expression expression(sourceExpression);
	return expression.getResult();
}
