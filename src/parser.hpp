#pragma once
#include "node.hpp"
#include "stringiterator.hpp"
#include <map>
#include <tuple>

#define StandartMethod(action) [](const double l, const double r) -> double {\
	return l action r;\
}
#define FunctionMethod(action) [](const double l, const double r) -> double {\
	return action(l, r);\
}

class Parser {
public:
	Parser() noexcept;
	NodePtr parse(const std::string& expression) noexcept;

	void registerOperation(const std::string& definition,
			const Operation::Method& method,
			const Node::Priority priority) noexcept;
	void registerBrackets(const std::string& left, const std::string& right) noexcept;

private:
	static bool isInsagnificant(const char symbol) noexcept;
	static bool isMinus(const char symbol) noexcept;
	static bool isDigit(const char symbol) noexcept;
	static bool isDot(const char symbol) noexcept;
	static bool isValidNumeric(const char symbol) noexcept;

	static bool isContains(std::string::const_iterator iterator,
			const std::string::const_iterator& terminator,
			const std::string& target) noexcept;
	
	std::map<std::string, std::tuple<Operation::Method, Node::Priority>> operations;
	std::map<std::string, std::string> brackets;

	void registerDefaults() noexcept;
	
	bool parseRecursive(StringIterator& source, RootPtr& root) noexcept;
	bool parseRecursiveBracketsNext(StringIterator& source, RootPtr& root) noexcept;
	bool parseRecursiveNumberNext(StringIterator& source, RootPtr& root) noexcept;
	bool parseRecursiveOperationNext(StringIterator& source, RootPtr& root) noexcept;

	void skipInsagnificants(StringIterator& source) noexcept;

	NumberPtr parseNumber(StringIterator& source) noexcept;
	OperationPtr parseOperation(StringIterator& source) noexcept;
	BracketsPtr parseBrackets(StringIterator& source) noexcept;
};
