#ifndef NONDEFRAUDAT_STRING_MATH_EXPRESSION_H_
#define NONDEFRAUDAT_STRING_MATH_EXPRESSION_H_

#include "lexem.h"
#include "operation.h"
#include "bracket.h"

#include <list> 
#include <ostream>

namespace nd_sm {
	class expression {
		std::list<lexem> expression_;
		std::list<lexem> reverse_polish_notation_;
		std::list<lexem> result_;

	protected:
		expression(std::list<lexem> expression);

	public:
		expression(const char* definition);
		expression(const wchar_t* definition);

		friend std::ostream& operator<<(std::ostream& os, const expression&);

		expression result();

		char* c_str();
		wchar_t* c_wstr();
	};
}

#endif