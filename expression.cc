#define _USE_MATH_DEFINES 

#include "expression.h"

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

	expression::expression(std::list<lexem> expression) noexcept {
		expression_ = result_ = expression;
	}

	// 

	inline std::list<lexem> extract_expression(const char* definition);
	inline std::list<lexem> calculate(std::list<lexem> expression);
	
	expression::expression(const char* definition) {
		expression_ = extract_expression(definition);
		result_ = calculate(expression_);
	}

	inline bool is_exist(const char* ptr);
	inline bool is_minus(const char* ptr);
	inline bool is_digit(const char* ptr);
	inline bool is_space(const char* ptr);
	inline void shift_ptr(const char*& ptr);
	inline lexem extract_number_and_shift_ptr(const char*& ptr);
	inline lexem extract_lexem_and_shift_ptr(const char*& ptr);

	inline std::list<lexem> extract_expression(const char* it) {
		std::list<lexem> expression;
		while (is_exist(it)) {
			if (is_space(it)) {
				shift_ptr(it);
			}
			else if (is_digit(it) or is_minus(it) 
				and (expression.empty() 
					or expression.back().type() != lexem_type::number)) {

				expression.push_back(extract_number_and_shift_ptr(it));
			}
			else {
				expression.push_back(extract_lexem_and_shift_ptr(it));
			}
		}
		return expression;
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

	inline bool is_dot(const char* ptr) {
		return *ptr == '.';
	}

	inline bool is_space(const char* ptr) {
		return *ptr == ' ' or *ptr == '\t';
	}

	inline void shift_ptr(const char*& ptr) {
		ptr++;
	}

	inline lexem extract_number_and_shift_ptr(const char*& ptr) {
		std::string number = "";
		int minus_count = 0;
		for (ptr; is_minus(ptr); ptr++) {
			minus_count++;
		}
		if (minus_count % 2) {
			number += '-';
		}
		for (ptr; is_digit(ptr) or is_dot(ptr); ptr++) {
			static bool dot_counter = 0;
			if (is_dot(ptr)) {
				if (dot_counter) {
					throw std::exception("Float value must have only one '.' in definition!");
				}
				dot_counter++;
			}
			number += *ptr;
		}
		return lexem(std::stod(number));
	}

	std::list<lexem> define_standart_lexems();
	inline bool is_values_match(const char*, const char*);

	inline lexem extract_lexem_and_shift_ptr(const char*& ptr) {
		static std::list<lexem> defined_lexems = define_standart_lexems();
		for (std::list<lexem>::iterator it = defined_lexems.begin();
			it != defined_lexems.end(); it++) {

			if (is_values_match(it->definition(), ptr)) {
				ptr += std::strlen(it->definition());
				return *it;
			}
		}
		throw std::exception("No matches");
	}

	inline std::list<lexem> define_standart_brackets();
	inline std::list<lexem> define_standart_operations();

	inline std::list<lexem> define_standart_lexems() {
		std::list<lexem> defined_lexems
			= define_standart_brackets();
		defined_lexems.splice(defined_lexems.end(), define_standart_operations());
		return defined_lexems;
	}

	inline std::list<lexem> define_standart_brackets() {
		std::list<lexem> standart_brackets;

		standart_brackets.push_back(
			lexem("(", bracket_t(bracket_orientation::left)));
		standart_brackets.push_back(
			lexem(")", bracket_t(bracket_orientation::right)));

		standart_brackets.push_back(
			lexem("[", bracket_t(bracket_orientation::left)));
		standart_brackets.push_back(
			lexem("]", bracket_t(bracket_orientation::right)));

		return standart_brackets;
	}

	inline std::list<lexem> define_standart_operations() {
		std::list<lexem> standart_operations;

		standart_operations.push_back(
			lexem("+", operation_t(2, [](double args[]) -> double { return args[0] + args[1]; }, 1)));
		standart_operations.push_back(
			lexem("-", operation_t(2, [](double args[]) -> double { return args[0] - args[1]; }, 1)));

		standart_operations.push_back(
			lexem("*", operation_t(2, [](double args[]) -> double { return args[0] * args[1]; }, 2)));
		standart_operations.push_back(
			lexem("/", operation_t(2, [](double args[]) -> double { return args[0] / args[1]; }, 2)));
		
		standart_operations.push_back(
			lexem("%", operation_t(2, [](double args[]) -> double { return std::fmod(args[1], args[0]); }, 3)));
		standart_operations.push_back(
			lexem("^", operation_t(2, [](double args[]) -> double { return std::pow(args[0], args[1]); }, 3)));
		standart_operations.push_back(
			lexem("sqrt", operation_t(1, [](double args[]) -> double { return std::sqrt(args[0]); }, 3)));

		standart_operations.push_back(
			lexem("sin", operation_t(1, [](double args[]) -> double { return std::sin(args[0]); }, 3)));
		standart_operations.push_back(
			lexem("cos", operation_t(1, [](double args[]) -> double { return std::cos(args[0]); }, 3)));
		standart_operations.push_back(
			lexem("tan", operation_t(1, [](double args[]) -> double { return std::tan(args[0]); }, 3)));

		standart_operations.push_back(
			lexem("asin", operation_t(1, [](double args[]) -> double { return std::asin(args[0]); }, 3)));
		standart_operations.push_back(
			lexem("acos", operation_t(1, [](double args[]) -> double { return std::acos(args[0]); }, 3)));
		standart_operations.push_back(
			lexem("atan", operation_t(1, [](double args[]) -> double { return std::atan(args[0]); }, 3)));

		standart_operations.push_back(
			lexem("log", operation_t(2, [](double args[]) -> double { return std::log(args[1]) / std::log(args[0]); }, 3)));
		standart_operations.push_back(
			lexem("ln", operation_t(1, [](double args[]) -> double { return std::log(args[0]); }, 3)));
		standart_operations.push_back(
			lexem("lg", operation_t(1, [](double args[]) -> double { return std::log10(args[0]); }, 3)));

		standart_operations.push_back(
			lexem("abs", operation_t(1, [](double args[]) -> double { return std::abs(args[0]); }, 3)));

		standart_operations.push_back(
			lexem("pi", operation_t(0, [](double args[]) -> double { return M_PI; }, 4)));
		standart_operations.push_back(
			lexem("inf", operation_t(0, [](double args[]) -> double { return std::numeric_limits<double>::infinity(); }, 4)));
		standart_operations.push_back(
			lexem("e", operation_t(0, [](double args[]) -> double { return M_E; }, 4)));

		return standart_operations;
	}

	inline bool is_values_match(const char* definition, const char* ptr) {
		while (is_exist(definition)) {
			if (*definition++ != *ptr++) {
				return false;
			}
		}
		return true;
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

	inline std::list<lexem> generate_rpn(std::list<lexem> expression);
	inline std::list<lexem> extract_result(std::list<lexem> expression);

	inline std::list<lexem> calculate(std::list<lexem> expression) {
		return extract_result(generate_rpn(expression));
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

	// 

	char* convert_to_c_str(const wchar_t* definition);
	
	expression::expression(const wchar_t* definition) : expression(convert_to_c_str(definition)) { }

	char* convert_to_c_str(const wchar_t* definition) {
		size_t length = std::wcslen(definition);
		char* converted_definition = new char[length + 1]; // +1 for '\0'
		for (int i = 0; i < length; i++) {
			if (definition[i] > 255) {
				throw std::exception("UNICODE character detected, ASCII character expected");
			}
			converted_definition[i] = static_cast<char>(definition[i]);
		}
		converted_definition[length] = '\0';
		return converted_definition;
	}


	expression expression::result() noexcept {
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
		for (size_t i = 0; i < length; i++) {
			c_wstr[i] = static_cast<wchar_t>(c_str[i]);
		}
		c_wstr[length] = L'\0';
		return c_wstr;
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