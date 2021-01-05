#ifndef NONDEFRAUDAT_STRING_MATH_MATH_FUNCTION_H_
#define NONDEFRAUDAT_STRING_MATH_MATH_FUNCTION_H_

#include <cstdarg>

namespace nd_sm {

	class operation_t {
		const char* definition_;
		size_t count_of_args_;
		double(*action_)(double[]);
		size_t priority_;

	public:
		explicit operation_t(const char* definition,
			size_t count_of_args, 
			double(*action)(double[]),
			size_t priority) noexcept;
		
		const char* definition() const;
		size_t count_of_args() const;
		double execute(double args[]) noexcept;
		size_t priority() const;
	};

}

#endif