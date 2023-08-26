#pragma once
#include "expressionnode.hpp"
#include <map>

class OperationRegister {
public:
	static struct PriorityNode {
		size_t priority = -1;
		OperationNodePtr node = nullptr;
	} undefined;

	OperationRegister() : registered(), maxDefinitionSize(0) { initDefault(); }

	void add(const std::string& definition, const size_t defaultPriority,
			const OperationHandler& operationHandler) noexcept {
		if (definition.size() > maxDefinitionSize) {
			maxDefinitionSize = definition.size();
		}
		registered[definition] = { defaultPriority, operationHandler };
	}

	size_t getMaxDefinitionSize() const noexcept { return maxDefinitionSize; }
	PriorityNode get(const std::string& definition) const noexcept {
		if (registered.contains(definition)) {
			OperationRef ref = registered.at(definition);
			return { ref.defaultPriority, std::make_shared<OperationNode>(definition, ref.handler) };
		}
		return PriorityNode();
	}

private:
	struct OperationRef {
		size_t defaultPriority;
		OperationHandler handler;
	};
	std::map<std::string, OperationRef> registered;

	size_t maxDefinitionSize;

	void initDefault() noexcept;
};
