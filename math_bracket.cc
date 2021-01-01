#include "math_bracket.h"

namespace nd_sm {
	math_bracket::math_bracket(const char* definition, bracket_type type) noexcept {
		definition_ = definition;
		type_ = type;
	}

	const char* math_bracket::definition() const {
		return definition_;
	}

	bracket_type math_bracket::type() const {
		return type_;
	}
}