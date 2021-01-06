#include "expression.h"

#include "lexem.h"
#include "operation.h"
#include "bracket.h"

#include <list> 
#include <string>
#include <exception>
#include <iostream>
#include <cmath>

namespace nd_sm {
	
	inline std::list<lexem> extract_expression(const char* it);
	inline std::list<lexem> generate_rpn(std::list<lexem> expression);
	inline std::list<lexem> extract_result(std::list<lexem> rpn);

	expression::expression(std::list<lexem> expression) {
		expression_ = expression;
		reverse_polish_notation_ = generate_rpn(expression_);
		result_ = extract_result(reverse_polish_notation_);
	}

	expression::expression(const char* definition) {
		expression_ = extract_expression(definition);
		reverse_polish_notation_ = generate_rpn(expression_);
		result_ = extract_result(reverse_polish_notation_);
	}

	expression::expression(const wchar_t* definition) {
		size_t length = std::wcslen(definition);
		char* converted_definition = new char[length + 1]; // +1 for '\0'
		for (int i = 0; i < length; i++) {
			if (definition[i] > 255) {
				throw std::exception("UNICODE character detected, ASCII character expected");
			}
			converted_definition[i] = static_cast<char>(definition[i]);
		}
		converted_definition[length] = '\0';
		*this = expression(converted_definition);
	}

	expression expression::result() {
		return expression(result_);
	}

	char* expression::c_str() {
		std::list<lexem>::const_iterator it = expression_.begin();
		std::string converter = "";
		if (it != expression_.end()) {
			converter = it->definition();
			it++;
			for (it; it != expression_.end(); it++) {
				converter += it->definition();
				converter += ' ';
			}
		}
		char* c_str = new char[converter.length() + 1]; // +1 for '\0'
		for (int i = 0; i < converter.length(); i++) {
			c_str[i] = converter[i];
		}
		c_str[converter.length()] = '\0';
		return c_str;
	}

	wchar_t* expression::c_wstr() {
		const char* c_str = expression::c_str();
		size_t length = std::strlen(c_str);
		wchar_t* c_wstr = new wchar_t[length + 1]; // +1 for '\0'
		for (size_t i = 0; i <= length; i++) {
			c_wstr[i] = static_cast<wchar_t>(c_wstr[i]);
		}
		return c_wstr;
	}

	std::list<lexem> define_standart_brackets();
	std::list<lexem> define_standart_functions();
	inline bool is_exist(const char* ptr);
	inline bool is_minus(const char* ptr);
	inline bool is_digit(const char* ptr);
	inline lexem extract_number_and_shift_ptr(const char*& ptr);
	inline bool is_bracket(std::list<lexem> standart_brackets,
		const char* ptr);
	inline lexem extract_bracket_and_shift_ptr(std::list<lexem> standart_brackets,
		const char*& ptr);
	inline bool is_operation(std::list<lexem> standart_functions, 
		const char* ptr);
	inline lexem extract_operation_and_shift_ptr(std::list<lexem> standart_functions,
		const char*& ptr);
	inline bool is_space(const char* ptr);
	inline void shift_ptr(const char*& ptr);

	inline std::list<lexem> extract_expression(const char* it) {
		static std::list<lexem> standart_brackets = define_standart_brackets();
		
		static std::list<lexem> standart_functions = define_standart_functions();
		std::list<lexem> expression;

		while (is_exist(it)) {
			if (is_digit(it) or is_minus(it) and
				(expression.empty() or expression.back().type() != lexem_type::number)) {
				
				expression.push_back(extract_number_and_shift_ptr(it));
			}
			else if (is_bracket(standart_brackets, it)) {
				expression.push_back(extract_bracket_and_shift_ptr(standart_brackets, it));
			}
			else if (is_operation(standart_functions, it)) {
				expression.push_back(extract_operation_and_shift_ptr(standart_functions, it));
			}
			else if (is_space(it)) {
				shift_ptr(it);
			}
			else {
				throw std::exception("Incorrect expression definition");
			}
		}
		
		return expression;
	}

	std::list<lexem> define_standart_brackets() {
		std::list<lexem> standart_brackets;

		standart_brackets.push_back(
			lexem("(", bracket_t(bracket_orientation::left)));
		standart_brackets.push_back(
			lexem(")", bracket_t(bracket_orientation::right)));

		return standart_brackets;
	}

	std::list<lexem> define_standart_functions() {
		std::list<lexem> standart_functions;

		standart_functions.push_back(
			lexem("+", operation_t(2, [](double args[]) -> double { return args[0] + args[1]; }, 1)));
		standart_functions.push_back(
			lexem("-", operation_t(2, [](double args[]) -> double { return args[0] - args[1]; }, 1)));
		standart_functions.push_back(
			lexem("*", operation_t(2, [](double args[]) -> double { return args[0] * args[1]; }, 2)));
		standart_functions.push_back(
			lexem("/", operation_t(2, [](double args[]) -> double { return args[0] / args[1]; }, 2)));

		return standart_functions;
	}

	inline bool is_exist(const char* ptr) {
		return ptr and *ptr;
	}

	inline bool is_minus(const char* ptr) {
		return *ptr == '-';
	}

	inline bool is_digit(const char* ptr) {
		unsigned char value = *ptr - '0';
		return value >= 0 and value <= 9;
	}

	inline bool is_dot(const char* ptr);

	inline lexem extract_number_and_shift_ptr(const char*& ptr) {
		std::string number = "";
		for (ptr; is_digit(ptr) or is_dot(ptr) or is_minus(ptr); ptr++) {
			bool dot_counter = 0;
			if (is_dot(ptr)) {
				if (dot_counter) {
					throw std::exception("Float value must have only one '.' in definition!");
				}
				dot_counter++;
			}
			bool minus_counter = 0;
			if (is_minus(ptr)) {
				if (minus_counter) {
					minus_counter = 0;
				}
				minus_counter++;
			}
			else {
				if (minus_counter) {
					number += '-';
					minus_counter = 0;
				}
				number += *ptr;
			}
		}
		return lexem(std::stod(number));
	}

	inline bool is_values_match(const char* definition, const char* ptr);

	inline bool is_bracket(std::list<lexem> standart_brackets,
		const char* ptr) {

		static std::list<lexem>::iterator it;
		for (it = standart_brackets.begin(); it != standart_brackets.end(); it++) {
			if (is_values_match(it->definition(), ptr)) {
				return true;
			}
		}
		return false;

	}

	inline lexem extract_bracket_and_shift_ptr(std::list<lexem> standart_brackets,
		const char*& ptr) {

		static std::list<lexem>::iterator it;
		for (it = standart_brackets.begin(); it != standart_brackets.end(); it++) {
			if (is_values_match(it->definition(), ptr)) {
				ptr += std::strlen(it->definition());
				return *it;
			}
		}
		throw std::exception("No matches");
	}


	inline bool is_values_match(const char* definition, const char* ptr) {
		while (is_exist(definition)) {
			if (*definition++ != *ptr++) {
				return false;
			}
		}
		return true;
	}

	inline bool is_operation(std::list<lexem> standart_functions,
		const char* ptr) {

		static std::list<lexem>::iterator it;
		for (it = standart_functions.begin(); it != standart_functions.end(); it++) {
			if (is_values_match(it->definition(), ptr)) {
				return true;
			}
		}
		return false;
	}

	inline lexem extract_operation_and_shift_ptr(std::list<lexem> standart_functions,
		const char*& ptr) {

		static std::list<lexem>::iterator it;
		for (it = standart_functions.begin(); it != standart_functions.end(); it++) {
			if (is_values_match(it->definition(), ptr)) {
				ptr += std::strlen(it->definition());
				return *it;
			}
		}
		throw std::exception("No matches");
	}

	inline bool is_dot(const char* ptr) {
		return *ptr == '.';
	}

	inline bool is_space(const char* ptr) {
		return *ptr == ' ' or *ptr == '\t';
	}

	inline void shift_ptr(const char*& ptr) {
		ptr++;
	}

	inline std::list<lexem> generate_rpn(std::list<lexem> expression) {
		
		std::list<lexem> rpn;
		std::list<lexem> stack;
		
		static std::list<lexem>::iterator it;
		for (it = expression.begin(); it != expression.end(); it++) {
			switch (it->type()) {
			case lexem_type::number:
				rpn.push_back(*it);
				break;
			case lexem_type::bracket:
				if (it->bracket().orientation() == bracket_orientation::right) {
					while (stack.front().type() != it->type()) {
						rpn.push_back(stack.front());
						stack.pop_front();
					}
					stack.pop_front();
				}
				else {
					stack.push_front(*it);
				}
				break;
			case lexem_type::function:
				if (stack.empty()) {
					stack.push_front(*it);
				}
				else {
					while (!stack.empty() and stack.front().type() == lexem_type::function and
						stack.front().operation().priority() > it->operation().priority()) {
						rpn.push_back(stack.front());
						stack.pop_front();
					}
					stack.push_front(*it);
				}
				break;
			}
		}
		while (!stack.empty()) {
			rpn.push_back(stack.front());
			stack.pop_front();
		}
		return rpn;
	}

	inline std::list<lexem> extract_result(std::list<lexem> rpn) {
		std::list<lexem> stack;
		
		static std::list<lexem>::iterator it;
		for (it = rpn.begin(); it != rpn.end(); it++) {
			switch (it->type()) {
			case lexem_type::number:
				stack.push_front(*it);
				break;
			case lexem_type::function:
				size_t count = it->operation().count_of_args();
				double* args = new double[count];
				for (int i = count - 1; i >= 0; i--) {
					args[i] = stack.front().number();
					stack.pop_front();
				}
				stack.push_front(lexem(it->operation().execute(args)));
				delete[] args;
				break;
			}
		}
		return stack;
	}

	std::ostream& operator<<(std::ostream& os, const expression& e) {
		std::list<lexem>::const_iterator it = e.expression_.begin();
		if (it != e.expression_.end()) {
			os << *it++;
			for (it; it != e.expression_.end(); it++) {
				os << ' ' << *it;
			}
		}
		return os;
	}
}