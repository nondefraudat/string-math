#include "mathexpression.hpp"

MathExpression::MathExpression(const std::string& mathExpression, const ExpressionParser& expressionParser) noexcept 
		: expressionParser(expressionParser) {
	root = this->expressionParser.parse(mathExpression);
}

double MathExpression::result() const noexcept {
	return root->getResult();
}

std::string MathExpression::definition() const noexcept {
	return root->getDefinition();
}
