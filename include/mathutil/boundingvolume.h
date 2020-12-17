/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __BOUNDINGVOLUME_H__
#define __BOUNDINGVOLUME_H__

#include "mathutildefinitions.h"
#include "umath.h"
#include "uvec.h"
#include "uquat.h"

#pragma warning(disable: 4251)
namespace umath {class ScaledTransform;};
namespace bounding_volume
{
	class DLLMUTIL Sphere
	{
	public:
		Sphere(const Vector3 &origin,float radius);
		Sphere();
		Vector3 origin;
		float radius;
	};
	class DLLMUTIL AABB
	{
	public:
		AABB(const Vector3 &min,const Vector3 &max);
		AABB();
		Vector3 min;
		Vector3 max;
		Vector3 GetCenter() const {return (min +max) /2.f;}
		Vector3 GetExtents() const {return (max -min) /2.f;}
		bool Intersects(const AABB &other) const;
		AABB Transform(const umath::ScaledTransform &pose);
		static void GetRotatedBounds(const Vector3 &min,const Vector3 &max,Mat4 rot,Vector3 *rmin,Vector3 *rmax)
		{
			rot = glm::inverse(rot);
			uvec::zero(rmin);
			uvec::zero(rmax);
			for(int i=0;i<3;i++)
			{
				for(int j=0;j<3;j++)
				{
					float e = rot[i][j] *min[j];
					float f = rot[i][j] *max[j];
					if(e < f)
					{
						(*rmin)[i] += e;
						(*rmax)[i] += f;
					}
					else
					{
						(*rmin)[i] += f;
						(*rmax)[i] += e;
					}
				}
			}
		}
	};
	class DLLMUTIL OBB
		: public AABB
	{
	public:
		OBB(const Vector3 &min,const Vector3 &max,const Quat &rot);
		OBB(const Vector3 &min,const Vector3 &max);
		OBB();
		Quat rotation;
	};
};
#pragma warning(default: 4251)

#endif
