#pragma once

#include "node.hpp"
#include <map>
#include <tuple>
#include <stack>

using OperationTemplates = std::map<std::string, std::tuple<int, OperationMethod>>;

class Parser {
public:
	Parser() noexcept;

	NodePtr parse(const std::string& expression) const noexcept;
	NumberPtr parseNumber(const std::string& definition) const noexcept;
	OperationPtr parseOperation(const std::string& definition) const noexcept;

	void defineOperationTemplate(const std::string& definition,
			const int priority, const OperationMethod& method) noexcept;

private:
	static bool isInsignificant(const char symbol) noexcept;
	static bool isDigit(const char symbol) noexcept;
	static bool isDot(const char symbol) noexcept;
	static bool isValid(const char symbol) noexcept;

	NumberPtr parseNumber(std::string::const_iterator& iterator,
			const std::string::const_iterator& terminator) const noexcept;
	OperationPtr parseOperation(std::string::const_iterator& iterator,
			const std::string::const_iterator& terminator) const noexcept;

	OperationTemplates operationTemplates;
	std::stack<OperationPtr> operationStack;
	std::stack<NumberPtr> numberStack;
};
