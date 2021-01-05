#include "bracket.h"

namespace nd_sm {
	bracket_t::bracket_t(const char* definition, bracket_orientation orientation) noexcept {
		definition_ = definition;
		orientation_ = orientation;
	}

	const char* bracket_t::definition() const {
		return definition_;
	}

	bracket_orientation bracket_t::orientation() const {
		return orientation_;
	}
}