#include "expression.h"

#include "lexem.h"
#include "math_function.h"
#include "math_bracket.h"

#include <list> 
#include <string>
#include <exception>
#include <iostream>

namespace nd_sm {
	
	inline std::list<lexem> extract_expression(const char* it);
	inline std::list<lexem> generate_rpn(std::list<lexem> expression);
	inline double extract_result(std::list<lexem> rpn);

	expression::expression(const char* definition) {
		expression_ = extract_expression(definition);
		reverse_polish_notation_ = generate_rpn(expression_);
		result_ = extract_result(reverse_polish_notation_);
	}

	std::list<lexem> define_standart_brackets();
	std::list<lexem> define_standart_functions();
	inline bool is_exist(const char* ptr);
	inline bool is_minus(const char* ptr);
	inline bool is_digit(const char* ptr);
	inline double extract_number_and_shift_ptr(const char*& ptr);
	inline bool is_bracket(std::list<lexem> standart_brackets,
		const char* ptr);
	inline math_bracket extract_bracket_and_shift_ptr(std::list<lexem> standart_brackets,
		const char*& ptr);
	inline bool is_function(std::list<lexem> standart_functions, 
		const char* ptr);
	inline math_function extract_function_and_shift_ptr(std::list<lexem> standart_functions,
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
				
				expression.push_back(lexem(extract_number_and_shift_ptr(it)));
			}
			else if (is_bracket(standart_brackets, it)) {
				expression.push_back(lexem(extract_bracket_and_shift_ptr(standart_brackets, it)));
			}
			else if (is_function(standart_functions, it)) {
				expression.push_back(lexem(extract_function_and_shift_ptr(standart_functions, it)));
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
			lexem(math_bracket("(", bracket_type::left)));
		standart_brackets.push_back(
			lexem(math_bracket(")", bracket_type::right)));

		return standart_brackets;
	}

	std::list<lexem> define_standart_functions() {
		std::list<lexem> standart_functions;

		standart_functions.push_back(
			lexem(math_function("+", 2, [](double args[]) -> double { return args[0] + args[1]; }, 1)));
		standart_functions.push_back(
			lexem(math_function("-", 2, [](double args[]) -> double { return args[0] - args[1]; }, 1)));
		standart_functions.push_back(
			lexem(math_function("*", 2, [](double args[]) -> double { return args[0] * args[1]; }, 2)));
		standart_functions.push_back(
			lexem(math_function("/", 2, [](double args[]) -> double { return args[0] / args[1]; }, 2)));

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

	inline double extract_number_and_shift_ptr(const char*& ptr) {
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
		return std::stod(number);
	}

	inline bool is_values_match(const char* definition, const char* ptr);

	inline bool is_bracket(std::list<lexem> standart_brackets,
		const char* ptr) {

		static std::list<lexem>::iterator it;
		for (it = standart_brackets.begin(); it != standart_brackets.end(); it++) {
			if (is_values_match(it->bracket().definition(), ptr)) {
				return true;
			}
		}
		return false;

	}

	inline math_bracket extract_bracket_and_shift_ptr(std::list<lexem> standart_brackets,
		const char*& ptr) {

		static std::list<lexem>::iterator it;
		for (it = standart_brackets.begin(); it != standart_brackets.end(); it++) {
			if (is_values_match(it->bracket().definition(), ptr)) {
				ptr += std::strlen(it->bracket().definition());
				return it->bracket();
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

	inline bool is_function(std::list<lexem> standart_functions,
		const char* ptr) {

		static std::list<lexem>::iterator it;
		for (it = standart_functions.begin(); it != standart_functions.end(); it++) {
			if (is_values_match(it->function().definition(), ptr)) {
				return true;
			}
		}
		return false;
	}

	inline math_function extract_function_and_shift_ptr(std::list<lexem> standart_functions,
		const char*& ptr) {

		static std::list<lexem>::iterator it;
		for (it = standart_functions.begin(); it != standart_functions.end(); it++) {
			if (is_values_match(it->function().definition(), ptr)) {
				ptr += std::strlen(it->function().definition());
				return it->function();
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
				if (it->bracket().type() == bracket_type::right) {
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
						stack.front().function().priority() > it->function().priority()) {
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

	inline double extract_result(std::list<lexem> rpn) {
		std::list<double> stack;
		
		static std::list<lexem>::iterator it;
		for (it = rpn.begin(); it != rpn.end(); it++) {
			switch (it->type()) {
			case lexem_type::number:
				stack.push_front(it->number());
				break;
			case lexem_type::function:
				size_t count = it->function().count_of_args();
				double* args = new double[count];
				for (int i = count - 1; i >= 0; i--) {
					args[i] = stack.front();
					stack.pop_front();
				}
				stack.push_front(it->function().execute(args));
				delete[] args;
				break;
			}
		}
		double result = stack.front();
		stack.pop_front();
		return result;
	}

	std::ostream& operator<<(std::ostream& os, const expression& e) {
		static std::list<lexem>::const_iterator it;
		for (it = e.expression_.begin(); it != e.expression_.end(); it++) {
			os << *it << ' ';
		}
		os << std::endl;
		return os;
	}

	void expression::test() {
		std::cout << "source expression:\n>>> ";
		static std::list<lexem>::const_iterator it;
		for (it = expression_.begin(); it != expression_.end(); it++) {
			std::cout << *it << ' ';
		}
		std::cout << std::endl << "rpn:\n>>> ";
		for (it = reverse_polish_notation_.begin(); it != reverse_polish_notation_.end(); it++) {
			std::cout << *it << ' ';
		}
		std::cout << std::endl
			<< "result:\n>>> " << result_ << std::endl;
	}
}