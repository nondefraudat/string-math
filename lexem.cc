#include "lexem.h"

#include "operation.h"
#include "bracket.h"

#include <exception>
#include <string>

namespace nd_sm {
	inline const char* extract_definition(double number);

	lexem::lexem(double number) noexcept : type_(lexem_type::number), number_(number) {
		definition_ = extract_definition(number);
 	}

	inline const char* extract_definition(double number) {
		std::string converter = std::to_string(number);
		char* definition = new char[converter.length() + 1]; // +1 for '\0'
		for (size_t i = 0; i < converter.length(); i++) {
			definition[i] = converter[i];
		}
		definition[converter.length()] = '\0';
		char* it = &definition[converter.length() - 1];
		while (*it == '0') {
			*it = '\0';
			it--;
		}
		if (*it == '.') {
			*it = '\0';
		}
		return definition;
	}

	lexem::lexem(const char* definition, operation_t operation) noexcept 
		: type_(lexem_type::function), definition_(definition), operation_(operation)  { }

	lexem::lexem(const char* definition, bracket_t bracket) noexcept 
		: type_(lexem_type::bracket), definition_(definition), bracket_(bracket) { }
	
	const char* lexem::definition() const {
		return definition_;
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
		os << l.definition();
		return os;
	}
}