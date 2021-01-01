#ifndef NONDEFRAUDAT_STRING_MATH_LEXEM_H_
#define NONDEFRAUDAT_STRING_MATH_LEXEM_H_

#include "math_function.h"
#include "math_bracket.h"

#include <exception>
#include <ostream>

namespace nd_sm {
	
	enum class lexem_type {
		number,
		function,
		bracket
	};

	class math_function;

	class lexem {
		lexem_type type_;
		union {
			double number_;
			math_function function_;
			math_bracket bracket_;
		};

	public:
		explicit lexem(double number) noexcept;
		explicit lexem(math_function function) noexcept;
		explicit lexem(math_bracket bracket) noexcept;

		lexem_type type() const;
		double number();
		math_function function();
		math_bracket bracket();

		friend std::ostream& operator<<(std::ostream& os, const lexem&);
	};

}

#endif