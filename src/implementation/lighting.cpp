// SPDX-FileCopyrightText: (c) 2019 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

module;

module pragma.math;

import :lighting;

float ulighting::calc_cone_falloff(const Vector3 &lightDir, const Vector3 &dirToLight, pragma::math::Degree outerCutoffAngle, pragma::math::Degree innerCutoffAngle)
{
	float cosCurAngle = uvec::dot(-dirToLight, lightDir);
	float cosInnerConeAngle = cos(pragma::math::deg_to_rad(innerCutoffAngle) / 2.f);
	float cosOuterConeAngle = cos(pragma::math::deg_to_rad(outerCutoffAngle) / 2.f);
	if(cosCurAngle <= cosOuterConeAngle)
		return 0.0;
	float cosOuterMinusInnerAngle = cosInnerConeAngle - cosOuterConeAngle;
	return pragma::math::clamp((cosCurAngle - cosOuterConeAngle) / cosOuterMinusInnerAngle, 0.f, 1.f);
}
