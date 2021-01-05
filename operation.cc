#include "operation.h"


namespace nd_sm {
	operation_t::operation_t(const char* definition,
		size_t count_of_args, 
		double(*action)(double[]),
		size_t priority) noexcept {

		definition_ = definition;
		count_of_args_ = count_of_args;
		action_ = action;
		priority_ = priority;
	}

	const char* operation_t::definition() const {
		return definition_;
	}

	size_t operation_t::count_of_args() const {
		return count_of_args_;
	}

	double operation_t::execute(double args[]) noexcept {
		return action_(args);
	}
	
	size_t operation_t::priority() const {
		return priority_;
	}
}