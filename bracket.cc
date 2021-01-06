#include "bracket.h"

namespace nd_sm {
	bracket_t::bracket_t(bracket_orientation orientation) noexcept 
		: orientation_(orientation) { }

	bracket_orientation bracket_t::orientation() const {
		return orientation_;
	}
}