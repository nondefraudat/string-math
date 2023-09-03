#pragma once

#include "operationregister.hpp"
#include <list>
#include <string>

class ExpressionParser {
public:
	ExpressionParser() noexcept = default;

	ExpressionNodePtr parse(const std::string& expression, const OperationRegister& operationRegister = OperationRegister()) noexcept;

private:
	class PriorityNode {
		
	};

	using NodeList = std::list<PriorityNode>;
	
	static NodeList defineNodes(const std::string& expression, const OperationRegister& operationRegister) noexcept;
	static OperationRegister::PriorityNode order(const NodeList& nodes) noexcept;

	static bool isDigit(const char symbol) noexcept { return symbol >= '0' and symbol <= '9'; }
	static bool isDot(const char symbol) noexcept { return symbol == '.'; }
	static bool isSpace(const char symbol) noexcept { return symbol == ' ' or symbol == '\t'; }
};
