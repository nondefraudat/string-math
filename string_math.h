#ifndef NONDEFRAUDAT_STRING_MATH_H_
#define NONDEFRAUDAT_STRING_MATH_H_

#define EXPORT __declspec(dllexport)

#include "expression.h"

namespace nd_sm {
	EXPORT expression calculate(expression);
}

#endif