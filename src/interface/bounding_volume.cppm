// SPDX-FileCopyrightText: (c) 2019 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

module;

#include "definitions.hpp"

export module pragma.math:bounding_volume;

export import :quaternion;
export import :vector;

#undef min
#undef max

export {
#pragma warning(disable : 4251)
	namespace umath {
		class ScaledTransform;
	};
	namespace bounding_volume {
		class DLLMUTIL Sphere {
		  public:
			Sphere(const Vector3 &origin, float radius);
			Sphere();
			Vector3 origin;
			float radius;
		};
		class DLLMUTIL AABB {
		  public:
			AABB(const Vector3 &min, const Vector3 &max);
			AABB();
			Vector3 min;
			Vector3 max;
			Vector3 GetCenter() const { return (min + max) / 2.f; }
			Vector3 GetExtents() const { return (max - min) / 2.f; }
			bool Intersects(const AABB &other) const;
			AABB Transform(const umath::ScaledTransform &pose);
			static void GetRotatedBounds(const Vector3 &min, const Vector3 &max, Mat4 rot, Vector3 *rmin, Vector3 *rmax)
			{
				rot = glm::inverse(rot);
				uvec::zero(rmin);
				uvec::zero(rmax);
				for(int i = 0; i < 3; i++) {
					for(int j = 0; j < 3; j++) {
						float e = rot[i][j] * min[j];
						float f = rot[i][j] * max[j];
						if(e < f) {
							(*rmin)[i] += e;
							(*rmax)[i] += f;
						}
						else {
							(*rmin)[i] += f;
							(*rmax)[i] += e;
						}
					}
				}
			}
		};
		class DLLMUTIL OBB : public AABB {
		  public:
			OBB(const Vector3 &min, const Vector3 &max, const Quat &rot);
			OBB(const Vector3 &min, const Vector3 &max);
			OBB();
			Quat rotation;
		};
	};
#pragma warning(default : 4251)
}
