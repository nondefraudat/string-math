#pragma once

#include "expressionparser.hpp"
#include <string>

class MathExpression {
public:
	MathExpression(const char* mathExpression) noexcept
			: MathExpression(std::string(mathExpression)) { }
	MathExpression(const std::string& mathExpression) noexcept
			: MathExpression(mathExpression, ExpressionParser()) { }
	explicit MathExpression(const std::string& mathExpression, const ExpressionParser& expressionParser) noexcept;

	double result() const noexcept;
	std::string definition() const noexcept;

private:
	ExpressionParser expressionParser;
	ExpressionNodePtr root;
};
