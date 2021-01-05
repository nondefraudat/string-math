#ifndef NONDEFRAUDAT_STRING_MATH_BRACKET_H_
#define NONDEFRAUDAT_STRING_MATH_BRACKET_H_

namespace nd_sm {

	enum class bracket_orientation {
		left,
		right
	};

	class bracket_t {
		const char* definition_;
		bracket_orientation orientation_;

	public:
		bracket_t(const char* definition, bracket_orientation orientation) noexcept;
		
		const char* definition() const;
		bracket_orientation orientation() const;
	};
}

#endif