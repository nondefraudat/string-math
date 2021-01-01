#include "lexem.h"

#include "math_function.h"

#include <exception>

namespace nd_sm {
	lexem::lexem(double number) noexcept : type_(lexem_type::number) {
		number_ = number;
	}

	lexem::lexem(math_function function) noexcept : type_(lexem_type::function) {
		function_ = function;
	}

	lexem::lexem(math_bracket bracket) noexcept : type_(lexem_type::bracket) {
		bracket_ = bracket;
	}
	
	lexem_type lexem::type() const {
		return type_;
	}

	double lexem::number() {
		if (type_ != lexem_type::number) {
			throw std::exception("Lexem isn't a number!");
		}
		return number_;
	}

	math_function lexem::function() {
		if (type_ != lexem_type::function) {
			throw std::exception("Lexem isn't a function!");
		}
		return function_;
	}

	math_bracket lexem::bracket() {
		if (type_ != lexem_type::bracket) {
			throw std::exception("Lexem isn't a bracket"); 
		}
		return bracket_;
	}

	std::ostream& operator<<(std::ostream& os, const lexem& l) {
		switch (l.type()) {
		case lexem_type::number:
			os << l.number_;
			break;
		case lexem_type::function:
			os << l.function_.definition();
			break;
		case lexem_type::bracket:
			os << l.bracket_.definition();
			break;
		}
		return os;
	}
}