#pragma once

#include "parser.hpp"
#include <string>
#include <memory>
#include <iostream>

std::ostream& operator<<(std::ostream& os, const class Expression& expression);

class Expression {
public:
	Expression(const std::string& expression, Parser parser = Parser()) noexcept;
	Expression(const char* expression) noexcept;

	bool isValid() const noexcept;

	std::string getDefinition() const noexcept;
	double getResult() const noexcept;

private:
	NodePtr root;
};

