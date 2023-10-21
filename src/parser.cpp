#include "parser.hpp"
#include <cmath>

using namespace std;

Parser::Parser() noexcept {
	registerDefaults();
}

NodePtr Parser::parse(const std::string& expression) noexcept {
	RootPtr root = make_shared<Root>();
	string::const_iterator iterator = expression.cbegin();
	string::const_iterator terminator = expression.cend();
	while (iterator != terminator) {
		const string::const_iterator last = iterator;
		NodePtr node = parseNext(iterator, terminator);
		if (node) {
			root->pushNode(node);
		}
		if (iterator == last) {
			break;
		}
	}
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

bool Parser::isInsagnificant(const char symbol) noexcept {
    return symbol == ' ' || symbol == '\t' || symbol == '\n';
}

bool Parser::isMinus(const char symbol) noexcept {
    return symbol == '-';
}

bool Parser::isDigit(const char symbol) noexcept {
    return symbol >= '0' && symbol <= '9';
}

bool Parser::isDot(const char symbol) noexcept {
    return symbol == '.';
}

bool Parser::isValidNumeric(const char symbol) noexcept {
    return isMinus(symbol) || isDot(symbol) || isDigit(symbol);
}

bool Parser::isContains(std::string::const_iterator iterator, const std::string::const_iterator &terminator, const std::string &target) noexcept {
	string::const_iterator targetIt;
	const string::const_iterator& targetEnd = target.cend();
	while (iterator != terminator && targetIt != targetEnd && *targetIt == *iterator) {
		targetIt++;
		iterator++;
	}
    return targetIt == targetEnd;
}

void Parser::registerDefaults() noexcept {
	registerOperation("+", StandartMethod(+), Node::Priority::LOW);
	registerOperation("-", StandartMethod(-), Node::Priority::LOW);
	registerOperation("*", StandartMethod(*), Node::Priority::NORMAL);
	registerOperation("/", StandartMethod(/), Node::Priority::NORMAL);
	registerOperation("^", FunctionMethod(powf64), Node::Priority::HIGH);
	registerBrackets("(", ")");
}

NodePtr Parser::parseNext(std::string::const_iterator &iterator,
		const std::string::const_iterator &terminator) noexcept {
	skipInsagnificants(iterator, terminator);
	NumberPtr number = parseNumber(iterator, terminator);
	if (number) {
		return number;
	}
	OperationPtr operation = parseOperation(iterator, terminator);
	if (operation) {
		return operation;
	}
	BracketsPtr brackets = parseBrackets(iterator, terminator);
	if (brackets) {
		return brackets;
	}
    return nullptr;
}

void Parser::skipInsagnificants(std::string::const_iterator &iterator,
		const std::string::const_iterator &terminator) noexcept {
	while(iterator != terminator && isInsagnificant(*iterator)) {
		iterator++;
	}
}

NumberPtr Parser::parseNumber(std::string::const_iterator &iterator,
		const std::string::const_iterator &terminator) noexcept {
	if (iterator == terminator || !isValidNumeric(*iterator)) {
		return nullptr;
	}
	std::string buffer("");
	if (isMinus(*iterator)) {
		buffer.push_back(*iterator++);
	}
	if (iterator == terminator) {
		return nullptr;
	}
	while (iterator != terminator &&
			(isDigit(*iterator) || isInsagnificant(*iterator))) {
		if (isInsagnificant(*iterator)) {
			iterator++;
		}
		else {
			buffer.push_back(*iterator++);
		}
	}
	if (iterator == terminator) {
		return make_shared<Number>(stod(buffer));
	}
    if (isDot(*iterator)) {
		if (buffer.empty() || buffer == "-") {
			buffer.push_back('0');
		}
		buffer.push_back(*iterator++);
	}
	while (iterator != terminator &&
			(isDigit(*iterator) || isInsagnificant(*iterator))) {
		if (isInsagnificant(*iterator)) {
			iterator++;
		}
		else {
			buffer.push_back(*iterator++);
		}
	}
	return make_shared<Number>(stod(buffer));
}

OperationPtr Parser::parseOperation(std::string::const_iterator &iterator, 
		const std::string::const_iterator &terminator) noexcept {
	if (iterator == terminator) {
		return nullptr;
	}
	string buffer("");
	string::const_iterator temp = iterator;
	while (temp != terminator) {
		buffer.push_back(*temp++);
		if (operations.contains(buffer)) {
			iterator = temp;
			Operation::Method method;
			Node::Priority priority;
			tie(method, priority) = operations.at(buffer);
			return make_shared<Operation>(buffer, method, priority);
		}
	}
    return nullptr;
}

BracketsPtr Parser::parseBrackets(std::string::const_iterator &iterator,
		const std::string::const_iterator &terminator) noexcept {
	if (iterator == terminator) {
		return nullptr;
	}
	string buffer("");
	string::const_iterator temp = iterator;
	while (temp != terminator && !brackets.contains(buffer)) {
		buffer.push_back(*temp++);
	}
	if (temp == terminator) {
    	return nullptr;
	}
	iterator = temp;
	size_t counter = 1;
	const string& target = brackets.at(buffer);
	string subexpression("");
	while (iterator != terminator && counter) {
		if (isContains(iterator, terminator, buffer)) {
			counter++;
			iterator += buffer.length();
		}
		else if(isContains(iterator, terminator, target)) {
			counter--;
			iterator += target.length();
		}
		else {
			subexpression.push_back(*iterator++);
		}
	}
	if (subexpression.empty()) {
		return nullptr;
	}
	Parser subparser;
	return make_shared<Brackets>(buffer, target, subparser.parse(subexpression));
}
