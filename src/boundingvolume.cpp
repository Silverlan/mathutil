/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mathutil/boundingvolume.h"

using namespace bounding_volume;

Sphere::Sphere(const Vector3 &_origin,float _radius)
	: origin(_origin),radius(_radius)
{}
Sphere::Sphere()
	: Sphere(Vector3{0.f,0.f,0.f},0.f)
{}

AABB::AABB(const Vector3 &_min,const Vector3 &_max)
	: min(_min),max(_max)
{}
AABB::AABB()
{}

OBB::OBB(const Vector3 &min,const Vector3 &max,const Quat &rot)
	: AABB(min,max),rotation(rot)
{}
OBB::OBB(const Vector3 &min,const Vector3 &max)
	: OBB(min,max,Quat{1.f,0.f,0.f,0.f})
{}
OBB::OBB()
{}
