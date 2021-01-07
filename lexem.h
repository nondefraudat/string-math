#ifndef NONDEFRAUDAT_STRING_MATH_LEXEM_H_
#define NONDEFRAUDAT_STRING_MATH_LEXEM_H_

#define EXPORT __declspec(dllexport)

#include "operation.h"
#include "bracket.h"

#include <exception>
#include <ostream>
#include <string>

namespace  nd_sm {
	EXPORT enum class lexem_type {
		number,
		function,
		bracket
	};

	EXPORT class lexem {
		lexem_type type_;
		const char* definition_;
		union {
			double number_;
			operation_t operation_;
			bracket_t bracket_;
		};

	public:
		explicit lexem(double number) noexcept;
		explicit lexem(const char* definition, operation_t operation) noexcept;
		explicit lexem(const char* definition, bracket_t bracket) noexcept;

		const char* definition() const;
		lexem_type type() const;
		double number();
		operation_t operation();
		bracket_t bracket();

		friend std::ostream& operator<<(std::ostream& os, const lexem&);
	};
}

#endif