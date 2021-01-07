#ifndef NONDEFRAUDAT_STRING_MATH_H_
#define NONDEFRAUDAT_STRING_MATH_H_

#define EXPORT __declspec(dllexport)

#include "expression.h"

namespace nd_sm {
	expression EXPORT calculate(expression);
}

#endif