#ifndef NONDEFRAUDAT_STRING_MATH_EXPRESSION_H_
#define NONDEFRAUDAT_STRING_MATH_EXPRESSION_H_

#include "lexem.h"
#include "math_function.h"

#include <list> 
#include <ostream>

namespace nd_sm {
	class expression {
		std::list<lexem> expression_;
		std::list<lexem> reverse_polish_notation_;
		double result_;

	public:
		expression(const char* definition);

		friend std::ostream& operator<<(std::ostream& os, const expression&);

		void test();
	};
}

#endif