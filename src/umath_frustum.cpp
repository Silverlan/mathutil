/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mathutil/umath_frustum.hpp"

void umath::frustum::get_near_plane_size(float fovRad,float nearZ,float aspectRatio,float &outNearW,float &outNearH)
{
	outNearH = (-(2 *tanf(fovRad /2.f) *nearZ)) *2.f;
	outNearW = outNearH *aspectRatio;
}
void umath::frustum::get_far_plane_size(float fovRad,float farZ,float aspectRatio,float &outFarW,float &outFarH)
{
	outFarH = (-(2 *tanf(fovRad /2.f) *farZ)) *2.f;
	outFarW = outFarH *aspectRatio;
}

Vector3 umath::frustum::get_far_plane_center(const Vector3 &pos,const Vector3 &forward,float farZ) {return pos +forward *farZ;}
Vector3 umath::frustum::get_near_plane_center(const Vector3 &pos,const Vector3 &forward,float nearZ) {return pos +forward *nearZ;}

std::array<Vector3,4> umath::frustum::get_far_plane_boundaries(const Vector3 &pos,const Vector3 &forward,const Vector3 &up,float fovRad,float farZ,float aspectRatio,float *outFarW=nullptr,float *outFarH=nullptr)
{
	auto right = -uvec::cross(up,forward);
	uvec::normalize(&right);
	float wFar,hFar;
	get_far_plane_size(fovRad,farZ,aspectRatio,wFar,hFar);

	auto fc = get_far_plane_center(pos,forward,farZ);

	auto uFar = up *hFar /2.f;
	auto rFar = right *wFar /2.f;
	std::array<Vector3,4> bounds = {
		Vector3(fc -uFar -rFar), // Bottom left of far plane
		Vector3(fc +uFar -rFar), // Top left of far plane
		Vector3(fc +uFar +rFar), // Top right of far plane
		Vector3(fc -uFar +rFar) // Bottom right of far plane
	};

	if(outFarW != NULL)
		*outFarW = wFar;
	if(outFarH != NULL)
		*outFarH = hFar;
	return bounds;
}

std::array<Vector3,4> umath::frustum::get_near_plane_boundaries(const Vector3 &pos,const Vector3 &forward,const Vector3 &up,float fovRad,float nearZ,float aspectRatio,float *outNearW,float *outNearH)
{
	auto right = -uvec::cross(up,forward);
	uvec::normalize(&right);
	float wNear,hNear;
	get_near_plane_size(fovRad,nearZ,aspectRatio,wNear,hNear);

	auto nc = get_near_plane_center(pos,forward,nearZ);

	auto uNear = up *hNear /2.f;
	auto rNear = right *wNear /2.f;
	std::array<Vector3,4> bounds = {
		Vector3(nc -uNear -rNear), // Bottom left of near plane
		Vector3(nc +uNear -rNear), // Top left of near plane
		Vector3(nc +uNear +rNear), // Top right of near plane
		Vector3(nc -uNear +rNear) // Bottom right of near plane
	};

	if(outNearW != NULL)
		*outNearW = wNear;
	if(outNearH != NULL)
		*outNearH = hNear;
	return bounds;
}

Vector3 umath::frustum::get_near_plane_point(const Vector3 &pos,const Vector3 &forward,const Vector3 &right,const Vector3 &up,float fovRad,float nearZ,float aspectRatio,const Vector2 &uv)
{
	auto center = get_near_plane_center(pos,forward,nearZ);
	auto wNear = 0.f;
	auto hNear = 0.f;
	get_near_plane_size(fovRad,nearZ,aspectRatio,wNear,hNear);
	center += right *-(wNear /2.f *(uv.x -0.5f)) +up *(hNear /2.f *(uv.y -0.5f));
	return center;
}
Vector3 umath::frustum::get_far_plane_point(const Vector3 &pos,const Vector3 &forward,const Vector3 &right,const Vector3 &up,float fovRad,float farZ,float aspectRatio,const Vector2 &uv)
{
	auto center = get_far_plane_center(pos,forward,farZ);
	auto wFar = 0.f;
	auto hFar = 0.f;
	get_far_plane_size(fovRad,farZ,aspectRatio,wFar,hFar);
	center += right *-(wFar /2.f *(uv.x -0.5f)) +up *(hFar /2.f *(uv.y -0.5f));
	return center;
}
