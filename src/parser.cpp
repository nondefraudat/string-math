#include "parser.hpp"
#include <cmath>

Parser::Parser() noexcept {
	registerDefaultOperations();
}

NodePtr Parser::parse(const std::string& expression) noexcept {
	RootPtr root;
	
	return root;
}

void Parser::registerOperation(const std::string& definition,
		const Operation::Method& method,
		const Node::Priority priority) noexcept {
	operations[definition] = { method, priority };	
}

void Parser::registerBrackets(const std::string& left,
		const std::string& right) noexcept {
	brackets[left] = right;
}

void Parser::registerDefaultOperations() noexcept {
	registerOperation("+", StandartMethod(+), Node::Priority::LOW);
	registerOperation("-", StandartMethod(-), Node::Priority::LOW);
	registerOperation("*", StandartMethod(*), Node::Priority::NORMAL);
	registerOperation("/", StandartMethod(/), Node::Priority::NORMAL);
	registerOperation("^", FunctionMethod(powf64), Node::Priority::HIGH);
}
