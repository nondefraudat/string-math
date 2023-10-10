#include "parser.hpp"
#include <stack>

using namespace std;
using NumberStack = stack<NumberPtr>;
using OperationStack = stack<OperationPtr>;

Parser::Parser() noexcept {
	defineOperationTemplate("+", 1, StandartOperationTemplate(+));
	defineOperationTemplate("-", 1, StandartOperationTemplate(-));
	defineOperationTemplate("*", 2, StandartOperationTemplate(*));
	defineOperationTemplate("/", 2, StandartOperationTemplate(/));
	defineOperationTemplate("^", 3, FunctionOperationTemplate(pow));
}

NodePtr Parser::parse(const std::string& expression) noexcept {
	string::const_iterator iterator = expression.cbegin();
	parseNext(iterator, expression.cend());
	clearStacks();
	return root;
}

NumberPtr Parser::parseNumber(const std::string& definition) const noexcept {
	int dotCounter = 0;
	std::string buffer("");
	for (const char symbol : definition) {
		if (isInsignificant(symbol)) {
			continue;
		}
		if (isDot(symbol) && dotCounter == 1) {
			return nullptr;
		}
		if (!isDigit(symbol)) {
			return nullptr;
		}
		buffer.push_back(symbol);
	}
	return make_shared<Number>(buffer);
}

OperationPtr Parser::parseOperation(const std::string& definition) const noexcept {
	if (!operationTemplates.contains(definition)) {
		return nullptr;
	}
	int priority;
	OperationMethod method;
	tie(priority, method) = operationTemplates.at(definition);
	return make_shared<Operation>(definition, priority, method);
}

void Parser::defineOperationTemplate(const std::string& definition, const int priority, const OperationMethod& method) noexcept {
	operationTemplates[definition] = tuple<int, OperationMethod>(priority, method);
}

bool Parser::isInsignificant(const char symbol) noexcept {
	return symbol == ' ' || symbol == '\t' || symbol == '\n';
}

bool Parser::isDigit(const char symbol) noexcept {
	return symbol >= '0' && symbol <= '9';
}

bool Parser::isDot(const char symbol) noexcept {
	return symbol == '.';
}

bool Parser::isValidNumeric(const char symbol) noexcept {
	return isDigit(symbol) || isDot(symbol) || isInsignificant(symbol);
}

void Parser::parseNext(std::string::const_iterator& iterator, const std::string::const_iterator& terminator) noexcept {
	skipInsegnificants(iterator, terminator);
	if (iterator == terminator) {
		return;
	}
	if (isValidNumeric(*iterator)) {
		NumberPtr number = parseNumber(iterator, terminator);
		if (!number) {
			return;
		}
		numberStack.push(number);
	}
	OperationPtr operation = parseOperation(iterator, terminator);
	if (!operation) {
		return;
	}
	if (operationStack.empty() ||
			operation->getPriority() < operationStack.top()->getPriority()) {
		operationStack.push(operation);
	}
	else if (!root) {
		if (numberStack.empty()) {
			return;
		}
		operation->setLeftNode(numberStack.top());
		numberStack.pop();
		NumberPtr number = parseNumber(iterator, terminator);
		if (!number) {
			return;
		}
		operation->setRightNode(number);
		root = operation;
	}
	else if (numberStack.empty()) {
		return;
	}
	else {
		operation->setLeftNode(numberStack.top());
		operation->setRightNode(root);
		root = operation;
	}
	parseNext(iterator, terminator);
}

void Parser::skipInsegnificants(std::string::const_iterator& iterator, const std::string::const_iterator& terminator) noexcept {
	while (iterator != terminator && isInsignificant(*iterator)) {
		iterator++;
	}
}

NumberPtr Parser::parseNumber(std::string::const_iterator& iterator, const std::string::const_iterator& terminator) noexcept {
	string buffer("");
	while (iterator != terminator && isValidNumeric(*iterator)) {
		buffer.push_back(*iterator);
		iterator++;
	}
	return parseNumber(buffer);
}

OperationPtr Parser::parseOperation(std::string::const_iterator& iterator, const std::string::const_iterator& terminator) noexcept {
	string buffer("");
	while (iterator != terminator) {
		buffer.push_back(*iterator);
		iterator++;
		OperationPtr operation = parseOperation(buffer);
		if (operation) {
			return operation;
		}
	}
	return nullptr;
}

void Parser::clearStacks() noexcept {
	while (!operationStack.empty()) {
		OperationPtr operation = operationStack.top();
		operationStack.pop();
		if (numberStack.empty()) {
			return;
		}
		operation->setLeftNode(numberStack.top());
		numberStack.pop();
		if (root) {
			operation->setRightNode(root);
			root = operation;
		}
		else if (numberStack.empty()) {
			return;
		}
		else {
			operation->setRightNode(numberStack.top());
			numberStack.pop();
		}
	}
	if (numberStack.size() == 1 && !root) {
		root = numberStack.top();
		numberStack.pop();
	}
}
