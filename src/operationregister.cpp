#include "operationregister.hpp"

using namespace std;

void OperationRegister::initDefault() noexcept {
	add("+", 1, [](const ExpressionNodePtr& left, const ExpressionNodePtr& right) noexcept -> double {
				return left->getResult() + right->getResult();
			});
	add("-", 1, [](const ExpressionNodePtr& left, const ExpressionNodePtr& right) noexcept -> double {
				return left->getResult() - right->getResult();
			});

	add("*", 2, [](const ExpressionNodePtr& left, const ExpressionNodePtr& right) noexcept -> double {
				return left->getResult() * right->getResult();
			});
	add("/", 2, [](const ExpressionNodePtr& left, const ExpressionNodePtr& right) noexcept -> double {
				return left->getResult() / right->getResult();
			});
	add("//", 2, [](const ExpressionNodePtr& left, const ExpressionNodePtr& right) noexcept -> double {
				return static_cast<int>(left->getResult()) / right->getResult();
			});
	add("%", 2, [](const ExpressionNodePtr& left, const ExpressionNodePtr& right) noexcept -> double {
				return static_cast<int>(left->getResult()) % static_cast<int>(right->getResult());
			});

	add("^", 3, [](const ExpressionNodePtr& left, const ExpressionNodePtr& right) noexcept -> double {
				return pow(left->getResult(), right->getResult());
			});
}
