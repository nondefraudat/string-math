#ifndef NONDEFRAUDAT_STRING_MATH_LEXEM_H_
#define NONDEFRAUDAT_STRING_MATH_LEXEM_H_

#include "operation.h"
#include "bracket.h"

#include <exception>
#include <ostream>

namespace nd_sm {
	
	enum class lexem_type {
		number,
		function,
		bracket
	};

	class operation_t;
	class bracket_t;

	class lexem {
		lexem_type type_;
		union {
			double number_;
			operation_t operation_;
			bracket_t bracket_;
		};

	public:
		explicit lexem(double number) noexcept;
		explicit lexem(operation_t operation) noexcept;
		explicit lexem(bracket_t bracket) noexcept;

		lexem_type type() const;
		double number();
		operation_t operation();
		bracket_t bracket();

		friend std::ostream& operator<<(std::ostream& os, const lexem&);
	};

}

#endif