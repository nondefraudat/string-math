#pragma once
#include "node.hpp"
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
	std::map<std::string, std::tuple<Operation::Method, Node::Priority>> operations;
	std::map<std::string, std::string> brackets;

	void registerDefaultOperations() noexcept;
	NodePtr parseNext() noexcept;
};
