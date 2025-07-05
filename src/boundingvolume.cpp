// SPDX-FileCopyrightText: (c) 2019 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

#include "mathutil/boundingvolume.h"
#include "mathutil/transform.hpp"
#include "mathutil/umath_geometry.hpp"

using namespace bounding_volume;

Sphere::Sphere(const Vector3 &_origin, float _radius) : origin(_origin), radius(_radius) {}
Sphere::Sphere() : Sphere(Vector3 {0.f, 0.f, 0.f}, 0.f) {}

AABB::AABB(const Vector3 &_min, const Vector3 &_max) : min(_min), max(_max) {}
AABB::AABB() {}

bool AABB::Intersects(const AABB &other) const { return umath::intersection::aabb_aabb(*this, other); }
AABB AABB::Transform(const umath::ScaledTransform &pose)
{
	// See http://www.realtimerendering.com/resources/GraphicsGems/gems/TransBox.c
	auto srcBounds = *this;
	auto &scale = pose.GetScale();
	srcBounds.min *= scale;
	srcBounds.max *= scale;

	auto rotMat = glm::mat3_cast(pose.GetRotation());
	auto &translation = pose.GetOrigin();

	AABB newBounds {translation, translation};
	for(uint8_t i = 0; i < 3; ++i) {
		for(uint8_t j = 0; j < 3; ++j) {
			auto a = rotMat[i][j] * srcBounds.min[j];
			auto b = rotMat[i][j] * srcBounds.max[j];
			if(a < b) {
				newBounds.min[i] += a;
				newBounds.max[i] += b;
			}
			else {
				newBounds.min[i] += b;
				newBounds.max[i] += a;
			}
		}
	}
	return newBounds;
}

OBB::OBB(const Vector3 &min, const Vector3 &max, const Quat &rot) : AABB(min, max), rotation(rot) {}
OBB::OBB(const Vector3 &min, const Vector3 &max) : OBB(min, max, Quat {1.f, 0.f, 0.f, 0.f}) {}
OBB::OBB() {}
