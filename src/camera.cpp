/*
* Copyright 2011-2013 Blender Foundation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "mathutil/camera.hpp"

float umath::camera::calc_fov_from_lens(umath::Millimeter sensorSize,umath::Millimeter focalLength,float aspectRatio)
{
	// Source: https://github.com/blender/blender/blob/79f99becafa3e2a2d7e0b6779961a4e2b58b371e/intern/cycles/blender/blender_camera.cpp#L468
	return 2.f *atanf((0.5f *sensorSize) /focalLength /aspectRatio);
}
float umath::camera::calc_aperture_size_from_fstop(float fstop,umath::Millimeter focalLength,bool orthographicCamera)
{
	// Source: https://github.com/blender/blender/blob/79f99becafa3e2a2d7e0b6779961a4e2b58b371e/intern/cycles/blender/blender_camera.cpp#L213
	fstop = fmax(fstop,1e-5f);

	auto apertureSize = 0.f;
	if(orthographicCamera)
		apertureSize = 1.0f /(2.0f *fstop);
	else
		apertureSize = (focalLength *1e-3f) /(2.0f *fstop);
	return apertureSize;
}
float umath::camera::calc_focal_length_from_fov(umath::Degree hfov,umath::Millimeter sensorSize)
{
	return (sensorSize /2.0f) /tanf(umath::deg_to_rad(hfov) *0.5f);
}
float umath::camera::calc_fov_from_focal_length(umath::Millimeter focalLength,umath::Millimeter sensorSize)
{
	return 2.0f *atanf((sensorSize /2.0f) /focalLength);
}
