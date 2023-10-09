#include "parser.hpp"
#include <stack>

using namespace std;
using NumberStack = stack<NumberPtr>;
using OperationStack = stack<OperationPtr>;

Parser::Parser() noexcept {
	defineOperationTemplate("+", 1, OperationMethodTemplate(+));
	defineOperationTemplate("-", 1, OperationMethodTemplate(-));
	defineOperationTemplate("*", 2, OperationMethodTemplate(*));
	defineOperationTemplate("/", 2, OperationMethodTemplate(/));
}

NodePtr Parser::parse(const std::string& expression) const noexcept {
	NodePtr root = nullptr;
	NumberStack numbers;
	OperationStack operations;
	string::const_iterator iterator = expression.cbegin();
	while (iterator != expression.cend()) {
		if (isInsignificant(*iterator)) {
			iterator++;
			continue;
		}
		if (isDigit(*iterator) || isDot(*iterator)) {
			NumberPtr number = parseNumber(iterator, expression.cend());
			if (!number) {
				return nullptr;
			}
			numbers.push(number);
			continue;
		}
		OperationPtr operation = parseOperation(iterator, expression.cend());
		if (!operation || numbers.empty()) {
			return nullptr;
		}
		if (operations.empty() || operation->getPriority() < operations.top()->getPriority()) {
			while (!operations.empty()) {
				if (numbers.empty()) {
					return nullptr;
				}
				OperationPtr operation = operations.top();
				operations.pop();
				operation->setLeftNode(numbers.top());
				numbers.pop();
				if (root) {
					operation->setRightNode(root);
					root = operation;
					continue;
				}
				else if (numbers.empty()) {
					return nullptr;
				}
				operation->setRightNode(numbers.top());
				numbers.pop();
				root = operation;
			}
			operations.push(operation);
		}
		else {
			operation->setLeftNode(numbers.top());
			numbers.pop();
			if (root) {
				operation->setRightNode(root);
				root = operation;
				continue;
			}
			else if (numbers.empty()) {
				return nullptr;
			}
			operation->setRightNode(numbers.top());
			numbers.pop();
			root = operation;
		}
	}
	while (!operations.empty()) {
		if (numbers.empty()) {
			return nullptr;
		}
		OperationPtr operation = operations.top();
		operations.pop();
		operation->setLeftNode(numbers.top());
		numbers.pop();
		if (root) {
			operation->setRightNode(root);
			root = operation;
			continue;
		}
		else if (numbers.empty()) {
			return nullptr;
		}
		operation->setRightNode(numbers.top());
		numbers.pop();
		root = operation;
	}
	if (!numbers.empty()) {
		return nullptr;
	}
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

bool Parser::isValid(const char symbol) noexcept {
	return isInsignificant(symbol) || isDigit(symbol) || isDot(symbol);
}

NumberPtr Parser::parseNumber(std::string::const_iterator& iterator, const std::string::const_iterator& terminator) const noexcept {
	string buffer("");
	while (iterator != terminator && isValid(*iterator)) {
		buffer.push_back(*iterator);
		iterator++;
	}
	return parseNumber(buffer);
}

OperationPtr Parser::parseOperation(std::string::const_iterator& iterator, const std::string::const_iterator& terminator) const noexcept {
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
