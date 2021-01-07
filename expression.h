#ifndef NONDEFRAUDAT_STRING_MATH_EXPRESSION_H_
#define NONDEFRAUDAT_STRING_MATH_EXPRESSION_H_

#include "lexem.h"
#include "operation.h"
#include "bracket.h"

#include <list> 
#include <string>
#include <exception>
#include <iostream>
#include <cmath>
#include <limits>

namespace nd_sm {
	class expression {
		std::list<lexem> expression_;
		std::list<lexem> result_;

	protected:
		expression(std::list<lexem> expression) noexcept;

	public:
		expression(const char* definition);
		expression(const wchar_t* definition);

		friend std::ostream& operator<<(std::ostream& os, const expression&);

		expression result() noexcept;

		char* c_str();
		wchar_t* c_wstr();
	};
}

#endif