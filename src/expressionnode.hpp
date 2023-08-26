#pragma once
#include <memory>
#include <string>

using ExpressionNodePtr = std::shared_ptr<class ExpressionNode>;
using OperationNodePtr = std::shared_ptr<class OperationNode>;
using OperationHandler = double(*)(const ExpressionNodePtr&, const ExpressionNodePtr&) noexcept;

class ExpressionNode {
public:
	explicit ExpressionNode(const std::string& definition) noexcept
			: definition(definition) { }
	virtual std::string getDefinition() const noexcept { return definition; }
	virtual double getResult() const noexcept = 0;

private:
	std::string definition;
};

class NumberNode : public ExpressionNode {
public:
	explicit NumberNode(const std::string& definition) noexcept
			: ExpressionNode(definition), value(std::stof(definition)) { }
	double getResult() const noexcept { return value; }

private:
	double value;
};

class OperationNode : public ExpressionNode {
public:
	explicit OperationNode(const std::string& definition, const OperationHandler& handler) noexcept
			: ExpressionNode(definition), handler(handler) { }
	std::string getDefinition() const noexcept { 
		return leftNode->getDefinition() + ExpressionNode::getDefinition() + rightNode->getDefinition(); 
	}
	double getResult() const noexcept { return handler(leftNode, rightNode); }

	void setLeftNode(const ExpressionNodePtr& expressionNode) noexcept { leftNode = expressionNode; }
	void setRightNode(const ExpressionNodePtr& expressionNode) noexcept { rightNode = expressionNode; }

private:
	OperationHandler handler;
	ExpressionNodePtr leftNode, rightNode;
};
