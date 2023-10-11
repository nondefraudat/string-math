#pragma once

#include <string>
#include <memory>

#define StandartOperationTemplate(action)\
[](const NodePtr& left, const NodePtr& right) -> double {\
	return left->getResult() action right->getResult();\
}
#define FunctionOperationTemplate(function)\
[](const NodePtr& left, const NodePtr& right) -> double {\
	return function(left->getResult(), right->getResult());\
}

using NodePtr = std::shared_ptr<class Node>;
using NumberPtr = std::shared_ptr<class Number>;
using OperationPtr = std::shared_ptr<class Operation>;

class Node {
public:
	explicit Node(const std::string& definition) noexcept;

	virtual std::string getDefinition() const noexcept;
	virtual double getResult() const noexcept = 0;

private:
	std::string definition;
};

class Number : public Node {
public:
	explicit Number(const double result) noexcept;
	explicit Number(const std::string& definition) noexcept;

	double getResult() const noexcept override;

private:
	double result;
};

using OperationMethod = double(*)(const NodePtr& left, const NodePtr& right);

class Operation : public Node {
public:
	explicit Operation(const std::string& definition, const int priority,
			const OperationMethod& method) noexcept;
	
	std::string getDefinition() const noexcept override;
	double getResult() const noexcept override;
	int getPriority() const noexcept;

	void setLeftNode(const NodePtr& node) noexcept;
	void setRightNode(const NodePtr& node) noexcept;

private:
	int priority;
	OperationMethod method;
	NodePtr left = nullptr;
	NodePtr right = nullptr;
};

class Brackets : public Node {
public:
	explicit Brackets(const std::string& left, const std::string& right) noexcept;

	std::string getDefinition() const noexcept override;
	double getResult() const noexcept override;

	void setChildNode(const NodePtr& node) noexcept;

private:
	NodePtr child;
};
