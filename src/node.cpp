#include "node.hpp"
#include <format>

using namespace std;

Node::Node(const std::string& definition) noexcept
		: definition(definition) { }

std::string Node::getDefinition() const noexcept {
	return definition;
}

Number::Number(const double result) noexcept
		: Node(to_string(result)), result(result) { }

Number::Number(const std::string& definition) noexcept
		: Node(definition), result(stod(definition)) { }

double Number::getResult() const noexcept {
	return result;
}

Operation::Operation(const std::string& definition,
		const int priority, const OperationMethod& method) noexcept
		: Node(definition), priority(priority), method(method) { }

std::string Operation::getDefinition() const noexcept {
	if (!left || !right) {
		return "";
	}
	return format("{} {} {}", left->getDefinition(),
			Node::getDefinition(), right->getDefinition());
}

double Operation::getResult() const noexcept {
	return method(left, right);
}

int Operation::getPriority() const noexcept {
	return priority;
}

void Operation::setLeftNode(const NodePtr& node) noexcept {
	left = node;
}

void Operation::setRightNode(const NodePtr& node) noexcept {
	right = node;
}

Brackets::Brackets(const std::string& left, const std::string& right) noexcept
		: Node(left + "{}" + right) { }

std::string Brackets::getDefinition() const noexcept {
	if (!child) {
		return "";
	}
	return format(Node::getDefinition(), child->getDefinition());
}

double Brackets::getResult() const noexcept {
	if (!child) {
		return numeric_limits<double>::quiet_NaN();
	}
	return child->getResult();
}

void Brackets::setChildNode(const NodePtr& node) noexcept {

}
