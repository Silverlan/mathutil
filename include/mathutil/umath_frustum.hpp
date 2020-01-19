/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __UMATH_FRUSTUM_HPP__
#define __UMATH_FRUSTUM_HPP__

#include "mathutildefinitions.h"
#include "umath.h"
#include "uvec.h"
#include <array>

namespace umath
{
	namespace frustum
	{
		DLLMUTIL Vector3 get_plane_center(const Vector3 &pos,const Vector3 &forward,float z);
		DLLMUTIL void get_plane_size(float fovRad,float z,float aspectRatio,float &outW,float &outH);
		DLLMUTIL std::array<Vector3,4> get_plane_boundaries(const Vector3 &pos,const Vector3 &forward,const Vector3 &up,float fovRad,float z,float aspectRatio,float *outFarW,float *outFarH);
		DLLMUTIL Vector3 get_plane_point(const Vector3 &pos,const Vector3 &forward,const Vector3 &right,const Vector3 &up,float fovRad,float z,float aspectRatio,const Vector2 &uv);
	};
};

#endif
