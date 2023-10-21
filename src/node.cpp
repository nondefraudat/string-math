#include "node.hpp"
#include <limits>

using namespace std;

const double Node::errorValue(numeric_limits<double>::quiet_NaN());

bool Node::pushNode(const NodePtr &node) noexcept {
    return false;
}

Node::Priority Node::getPriority() const noexcept {
    return Priority::ABSOLUT;
}

Root::Root() noexcept : trunk(nullptr) { }

double Root::calculate() const noexcept {
	if (!trunk) {
		return errorValue;
	}
    return trunk->calculate();
}

std::string Root::parseDefinition() const noexcept {
	if (!trunk) {
		return "";
	}
    return trunk->parseDefinition();
}

bool Root::pushNode(const NodePtr &node) noexcept {
    if (!trunk) {
		trunk = node;
		return true;
	}
	if (node->getPriority() >= trunk->getPriority()) {
		return trunk->pushNode(node);
	}
	if (node->pushNode(trunk)) {
		trunk = node;
		return true;
	}
	return false;
}

Number::Number(const double value) noexcept
		: value(value) {
	definition = to_string(value);
	definition.erase(definition.find_last_not_of('0') + 1, string::npos);
	definition.erase(definition.find_last_not_of('.') + 1, string::npos);
}

double Number::calculate() const noexcept {
    return value;
}

std::string Number::parseDefinition() const noexcept {
    return definition;
}

Operation::Operation(const std::string &definition,
		const Method &method, const Priority priority) noexcept
		: definition(" " + definition + " "), method(method),
		priority(priority),
		left(nullptr), right(nullptr) { }

double Operation::calculate() const noexcept {
	if (!left || !right) {
		return errorValue;
	}
    return method(left->calculate(), right->calculate());
}

std::string Operation::parseDefinition() const noexcept {
    if (!left || !right) {
		return "";
	}
	return left->parseDefinition() + definition + right->parseDefinition();
}

bool Operation::pushNode(const NodePtr &node) noexcept {
	if (!left) {
		left = node;
		return true;
	}
	if (!right) {
		right = node;
		return true;
	}
	if (node->getPriority() >= right->getPriority()) {
		return right->pushNode(node);
	}
	if (node->pushNode(right)) {
		right = node;
		return true;
	}
    return false;
}

Node::Priority Operation::getPriority() const noexcept {
    return priority;
}

Brackets::Brackets(const std::string &left, const std::string &right,
		const NodePtr& root) noexcept 
		: left(left + " "), right(" " + right), root(root) { }

double Brackets::calculate() const noexcept {
    if (!root) {
		return errorValue;
	}
	return root->calculate();
}

std::string Brackets::parseDefinition() const noexcept {
	if (!root) {
		return "";
	}
    return left + root->parseDefinition() + right;
}
