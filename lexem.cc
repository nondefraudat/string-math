#include "lexem.h"

#include "operation.h"
#include "bracket.h"

#include <exception>

namespace nd_sm {
	lexem::lexem(double number) noexcept : type_(lexem_type::number) {
		number_ = number;
	}

	lexem::lexem(operation_t operation) noexcept : type_(lexem_type::function) {
		operation_ = operation;
	}

	lexem::lexem(bracket_t bracket) noexcept : type_(lexem_type::bracket) {
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

	operation_t lexem::operation() {
		if (type_ != lexem_type::function) {
			throw std::exception("Lexem isn't a function!");
		}
		return operation_;
	}

	bracket_t lexem::bracket() {
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
			os << l.operation_.definition();
			break;
		case lexem_type::bracket:
			os << l.bracket_.definition();
			break;
		}
		return os;
	}
}