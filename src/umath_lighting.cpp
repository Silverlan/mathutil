/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mathutil/umath_lighting.hpp"

float ulighting::calc_cone_falloff(const Vector3 &lightDir, const Vector3 &dirToLight, umath::Degree outerCutoffAngle, umath::Degree innerCutoffAngle)
{
	float cosCurAngle = uvec::dot(-dirToLight, lightDir);
	float cosInnerConeAngle = cos(umath::deg_to_rad(innerCutoffAngle) / 2.f);
	float cosOuterConeAngle = cos(umath::deg_to_rad(outerCutoffAngle) / 2.f);
	if(cosCurAngle <= cosOuterConeAngle)
		return 0.0;
	float cosOuterMinusInnerAngle = cosInnerConeAngle - cosOuterConeAngle;
	return umath::clamp((cosCurAngle - cosOuterConeAngle) / cosOuterMinusInnerAngle, 0.f, 1.f);
}
