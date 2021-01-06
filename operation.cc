#include "operation.h"


namespace nd_sm {
	operation_t::operation_t(size_t count_of_args, 
		double(*action)(double[]),
		size_t priority) noexcept 
		: count_of_args_(count_of_args), 
		action_(action), priority_(priority) { }

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