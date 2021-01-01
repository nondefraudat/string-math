#include "math_function.h"


namespace nd_sm {
	math_function::math_function(const char* definition, 
		size_t count_of_args, 
		double(*action)(double[]),
		size_t priority) noexcept {

		definition_ = definition;
		count_of_args_ = count_of_args;
		action_ = action;
		priority_ = priority;
	}

	const char* math_function::definition() const {
		return definition_;
	}

	size_t math_function::count_of_args() const {
		return count_of_args_;
	}

	double math_function::execute(double args[]) noexcept {
		return action_(args);
	}
	
	size_t math_function::priority() const {
		return priority_;
	}
}