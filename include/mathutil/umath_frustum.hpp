// SPDX-FileCopyrightText: (c) 2019 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

#ifndef __UMATH_FRUSTUM_HPP__
#define __UMATH_FRUSTUM_HPP__

#include "mathutildefinitions.h"
#include "umath.h"
#include "uvec.h"
#include <array>

namespace umath {
	namespace frustum {
		DLLMUTIL Vector3 get_plane_center(const Vector3 &pos, const Vector3 &forward, float z);
		DLLMUTIL void get_plane_size(float fovRad, float z, float aspectRatio, float &outW, float &outH);
		DLLMUTIL std::array<Vector3, 4> get_plane_boundaries(const Vector3 &pos, const Vector3 &forward, const Vector3 &up, float fovRad, float z, float aspectRatio, float *outFarW, float *outFarH);
		DLLMUTIL Vector3 get_plane_point(const Vector3 &pos, const Vector3 &forward, const Vector3 &right, const Vector3 &up, float fovRad, float z, float aspectRatio, const Vector2 &uv);
	};
};

#endif
