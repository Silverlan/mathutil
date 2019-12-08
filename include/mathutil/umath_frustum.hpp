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
		DLLMUTIL Vector3 get_far_plane_center(const Vector3 &pos,const Vector3 &forward,float farZ);
		DLLMUTIL Vector3 get_near_plane_center(const Vector3 &pos,const Vector3 &forward,float nearZ);
		DLLMUTIL void get_near_plane_size(float fovRad,float nearZ,float aspectRatio,float &outNearW,float &outNearH);
		DLLMUTIL void get_far_plane_size(float fovRad,float farZ,float aspectRatio,float &outFarW,float &outFarH);
		DLLMUTIL std::array<Vector3,4> get_far_plane_boundaries(const Vector3 &pos,const Vector3 &forward,const Vector3 &up,float fovRad,float farZ,float aspectRatio,float *outFarW,float *outFarH);
		DLLMUTIL std::array<Vector3,4> get_near_plane_boundaries(const Vector3 &pos,const Vector3 &forward,const Vector3 &up,float fovRad,float nearZ,float aspectRatio,float *outNearW,float *outNearH);
		DLLMUTIL Vector3 get_near_plane_point(const Vector3 &pos,const Vector3 &forward,const Vector3 &right,const Vector3 &up,float fovRad,float nearZ,float aspectRatio,const Vector2 &uv);
		DLLMUTIL Vector3 get_far_plane_point(const Vector3 &pos,const Vector3 &forward,const Vector3 &right,const Vector3 &up,float fovRad,float farZ,float aspectRatio,const Vector2 &uv);
	};
};

#endif
