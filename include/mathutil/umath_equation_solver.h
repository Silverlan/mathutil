/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __UMATH_EQUATION_SOLVER_H__
#define __UMATH_EQUATION_SOLVER_H__

#include "mathutildefinitions.h"
#include <cinttypes>
#include <array>

namespace umath
{
	DLLMUTIL int32_t solve_quadric(const std::array<double,3> &c,std::array<double,2> &s);
	DLLMUTIL int32_t solve_cubic(const std::array<double,4> &c,std::array<double,3> &s);
	DLLMUTIL int32_t solve_quartic(const std::array<double,5> &c,std::array<double,4> &s);
};

#endif
