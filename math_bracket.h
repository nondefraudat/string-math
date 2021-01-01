#ifndef NONDEFRAUDAT_STRING_MATH_BRACKET_H_
#define NONDEFRAUDAT_STRING_MATH_BRACKET_H_

namespace nd_sm {

	enum class bracket_type {
		left,
		right
	};

	class math_bracket {
		const char* definition_;
		bracket_type type_;

	public:
		math_bracket(const char* definition, bracket_type type) noexcept;
		
		const char* definition() const;
		bracket_type type() const;
	};
}

#endif