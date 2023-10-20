#pragma once
#include <memory>
#include <string>

using NodePtr = std::shared_ptr<class Node>;
using RootPtr = std::shared_ptr<class Root>;
using NumberPtr = std::shared_ptr<class Number>;
using OperationPtr = std::shared_ptr<class Operation>;

class Node {
public:
	enum class Priority : unsigned char {
		LOW = 1, NORMAL, HIGH, ABSOLUT
	};

	static const double errorValue;

	virtual double calculate() const noexcept = 0;
	virtual std::string parseDefinition() const noexcept = 0;

	virtual bool pushNode(const NodePtr& node) noexcept;
	virtual Priority getPriority() const noexcept;
};

class Root : public Node {
public:
	Root() noexcept;

	double calculate() const noexcept override;
	std::string parseDefinition() const noexcept override;
	bool pushNode(const NodePtr& node) noexcept override;

private:
	NodePtr trunk;
};

class Number : public Node {
public:
	Number(const double value) noexcept;
	double calculate() const noexcept override;
	std::string parseDefinition() const noexcept override;

private:
	double value;
	std::string definition;
};

class Operation : public Node {
public:
	using Method = double(*)(const double left, const double right);
	
	Operation(const std::string& definition,
			const Method& method) noexcept;
	double calculate() const noexcept override;
	std::string parseDefinition() const noexcept override;
	bool pushNode(const NodePtr& node) noexcept override;

private:
	std::string definition;
	Method method;
	NodePtr left, right;
};

class Brackets : public Node {
public:
	Brackets(const std::string& left,
			const std::string& right, const NodePtr& root) noexcept;
	double calculate() const noexcept override;
	std::string parseDefinition() const noexcept override;

private:
	std::string left, right;
	NodePtr root;
};
