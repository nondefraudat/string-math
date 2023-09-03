#include "expressionparser.hpp"

using namespace std;

ExpressionNodePtr ExpressionParser::parse(const std::string& expression, const OperationRegister& operationRegister) noexcept {
	const NodeList& nodes = defineNodes(expression, operationRegister);
	const OperationRegister::PriorityNode root = order(nodes);
	return root.node;
}

ExpressionParser::NodeList ExpressionParser::defineNodes(const std::string& expression, const OperationRegister& operationRegister) noexcept {
	NodeList nodes;
	const size_t maxDefinitionSize = operationRegister.getMaxDefinitionSize();
	size_t leftIndex, rightIndex;
	leftIndex = rightIndex = 0;
	while (leftIndex < expression.size()) {
		const char leftSymbol = expression[leftIndex];
		if (isDigit(leftSymbol) or isDot(leftSymbol)) {
			do {
				rightIndex++;
			} while (rightIndex < expression.size() and
					(isDigit(expression[rightIndex]) or isDot(expression[rightIndex])));
			const string number = expression.substr(leftIndex, leftIndex - rightIndex);
			nodes.push_back(OperationRegister::PriorityNode{ 0,  make_shared<NumberNode>(number) });
		}
	}
	return NodeList();
}

OperationRegister::PriorityNode ExpressionParser::order(const NodeList& nodes) noexcept {
	return OperationRegister::PriorityNode();
}
