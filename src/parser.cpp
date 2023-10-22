#include "parser.hpp"
#include <cmath>

using namespace std;

Parser::Parser() noexcept {
	registerDefaults();
}

NodePtr Parser::parse(const std::string& expression) noexcept {
	RootPtr root = make_shared<Root>();
	StringIterator source(expression);
	return parseRecursive(source, root) ? root : nullptr;
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
	string::const_iterator targetIt = target.cbegin();
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
	registerOperation("^", FunctionMethod(powf), Node::Priority::HIGH);
	registerBrackets("(", ")");
}

bool Parser::parseRecursive(StringIterator& source, RootPtr& root) noexcept {
	skipInsagnificants(source);
	return parseRecursiveBracketsNext(source, root) ||
			parseRecursiveNumberNext(source, root);
}

bool Parser::parseRecursiveBracketsNext(StringIterator &source, RootPtr &root) noexcept {
	if (source.atEnd()) {
		return false;
	}
	BracketsPtr brackets = parseBrackets(source);
	if (!brackets) {
		return false;
	}
	root->pushNode(brackets);
	skipInsagnificants(source);
    return source.atEnd() ? true : parseRecursiveOperationNext(source, root);
}

bool Parser::parseRecursiveNumberNext(StringIterator &source, RootPtr &root) noexcept {
    if (source.atEnd()) {
		return false;
	}
	NumberPtr number = parseNumber(source);
	if (!number) {
		return false;
	}
	root->pushNode(number);
	skipInsagnificants(source);
	return source.atEnd() ? true : parseRecursiveOperationNext(source, root);
}

bool Parser::parseRecursiveOperationNext(StringIterator &source, RootPtr &root) noexcept {
	if (source.atEnd()) {
		return false;
	}
	OperationPtr operation = parseOperation(source);
	if (!operation) {
		return false;
	}
	root->pushNode(operation);
	skipInsagnificants(source);
	return parseRecursiveBracketsNext(source, root) ||
			parseRecursiveNumberNext(source, root);
}

void Parser::skipInsagnificants(StringIterator& source) noexcept {
	while(!source.atEnd() && isInsagnificant(source.value())) {
		source.next();
	}
}

NumberPtr Parser::parseNumber(StringIterator& source) noexcept {
	if (source.atEnd() || !isValidNumeric(source.value())) {
		return nullptr;
	}
	StringIterator temp = source.createSubiterator();
	std::string buffer("");
	if (isMinus(temp.value())) {
		buffer.push_back(temp.valueNext());
	}
	if (temp.atEnd()) {
		return nullptr;
	}
	while (!temp.atEnd() &&
			(isDigit(temp.value()) || isInsagnificant(temp.value()))) {
		if (isDigit(temp.value())) {
			buffer.push_back(temp.value());
		}
		temp.next();
	}
	if (temp.atEnd()) {
		source.skipSubiterator(temp);
		return make_shared<Number>(stod(buffer));
	}
    if (isDot(temp.value())) {
		if (buffer.empty() || buffer == "-") {
			buffer.push_back('0');
		}
		buffer.push_back(temp.valueNext());
	}
	while (!temp.atEnd() &&
			(isDigit(temp.value()) || isInsagnificant(temp.value()))) {
		if (isDigit(temp.value())) {
			buffer.push_back(temp.value());
		}
		temp.next();
	}
	source.skipSubiterator(temp);
	return make_shared<Number>(stod(buffer));
}

OperationPtr Parser::parseOperation(StringIterator& source) noexcept {
	if (source.atEnd()) {
		return nullptr;
	}
	StringIterator temp = source.createSubiterator();
	string buffer("");
	while (!temp.atEnd()) {
		buffer.push_back(temp.valueNext());
		if (operations.contains(buffer)) {
			source.skipSubiterator(temp);
			Operation::Method method;
			Node::Priority priority;
			tie(method, priority) = operations.at(buffer);
			return make_shared<Operation>(buffer, method, priority);
		}
	}
    return nullptr;
}

BracketsPtr Parser::parseBrackets(StringIterator& source) noexcept {
	if (source.atEnd()) {
		return nullptr;
	}
	StringIterator temp = source.createSubiterator();
	string buffer("");
	while (temp.atEnd() && !brackets.contains(buffer)) {
		buffer.push_back(temp.valueNext());
	}
	if (temp.atEnd()) {
    	return nullptr;
	}
	source.skipSubiterator(temp);
	size_t counter = 1;
	const string& target = brackets.at(buffer);
	string subexpression("");
	while (temp.atEnd() && counter) {
		if (temp.skipDefinition(buffer)) {
			counter++;
		}
		else if(temp.skipDefinition(target)) {
			counter--;
		}
		else {
			subexpression.push_back(temp.valueNext());
		}
	}
	if (subexpression.empty()) {
		return nullptr;
	}
	source.skipSubiterator(temp);
	Parser subparser;
	return make_shared<Brackets>(buffer, target, subparser.parse(subexpression));
}
