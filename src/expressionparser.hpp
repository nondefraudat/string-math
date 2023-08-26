#pragma once

#include "operationregister.hpp"
#include <list>
#include <string>

class ExpressionParser {
public:
	ExpressionParser(const OperationRegister& operationRegister = OperationRegister()) noexcept
			: operationRegister(operationRegister) { }

	ExpressionNodePtr parse(const std::string& expression) noexcept;

private:
	using NodeList = std::list<OperationRegister::PriorityNode>;
	
	class Symbol {


	private:
		char value;
	};

	OperationRegister operationRegister;
	

	static bool isDigit(const char symbol) noexcept { return symbol >= '0' and symbol <= '9'; }
	static bool isDot(const char symbol) noexcept { return symbol == '.'; }
	static bool isSpace(const char symbol) noexcept { return symbol == ' ' or symbol == '\t'; }
};
