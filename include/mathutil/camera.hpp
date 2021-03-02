/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/.
*
* Copyright (c) 2021 Silverlan
*/

#ifndef __UMATH_CAMERA_HPP__
#define __UMATH_CAMERA_HPP__

#include "mathutil/mathutildefinitions.h"
#include "mathutil/umath.h"
#include "mathutil/uvec.h"

namespace umath::camera
{
	DLLMUTIL float calc_fov_from_lens(umath::Millimeter sensorSize,umath::Millimeter focalLength,float aspectRatio);
	DLLMUTIL float calc_aperture_size_from_fstop(float fstop,umath::Millimeter focalLength,bool orthographicCamera=false);
	DLLMUTIL float calc_focal_length_from_fov(umath::Degree hfov,umath::Millimeter sensorSize);
	DLLMUTIL float calc_fov_from_focal_length(umath::Millimeter focalLength,umath::Millimeter sensorSize);
};

#endif
