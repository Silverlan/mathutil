/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mathutil/umath_frustum.hpp"

void umath::frustum::get_plane_size(float fovRad,float z,float aspectRatio,float &outW,float &outH)
{
	static auto altMode = false;
	if(altMode == false)
	{
	outH = (-(2 *tanf(fovRad /2.0) *z)) *2.0;
	outW = outH *aspectRatio;
	}
	else
	{
	outW = 2.0 *tanf(fovRad /2.0) *z;
	outH = outW *(1.0 /aspectRatio);
	}
}

Vector3 umath::frustum::get_plane_center(const Vector3 &pos,const Vector3 &forward,float z) {return pos +forward *z;}

std::array<Vector3,4> umath::frustum::get_plane_boundaries(const Vector3 &pos,const Vector3 &forward,const Vector3 &up,float fovRad,float z,float aspectRatio,float *outFarW=nullptr,float *outFarH=nullptr)
{
	auto right = -uvec::cross(up,forward);
	uvec::normalize(&right);
	float w,h;
	get_plane_size(fovRad,z,aspectRatio,w,h);

	auto fc = get_plane_center(pos,forward,z);

	auto uFar = up *h /2.f;
	auto rFar = right *w /2.f;
	std::array<Vector3,4> bounds = {
		Vector3(fc -uFar -rFar), // Bottom left of far plane
		Vector3(fc +uFar -rFar), // Top left of far plane
		Vector3(fc +uFar +rFar), // Top right of far plane
		Vector3(fc -uFar +rFar) // Bottom right of far plane
	};

	if(outFarW != NULL)
		*outFarW = w;
	if(outFarH != NULL)
		*outFarH = h;
	return bounds;
}

Vector3 umath::frustum::get_plane_point(const Vector3 &pos,const Vector3 &forward,const Vector3 &right,const Vector3 &up,float fovRad,float z,float aspectRatio,const Vector2 &uv)
{
	auto center = get_plane_center(pos,forward,z);
	auto w = 0.f;
	auto h = 0.f;
	get_plane_size(fovRad,z,aspectRatio,w,h);
	center += right *-(w /2.f *(uv.x -0.5f)) +up *(h /2.f *(uv.y -0.5f));
	return center;
}
