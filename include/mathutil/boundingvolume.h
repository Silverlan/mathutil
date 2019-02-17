#ifndef __BOUNDINGVOLUME_H__
#define __BOUNDINGVOLUME_H__

#include "mathutildefinitions.h"
#include "umath.h"
#include "uvec.h"
#include "uquat.h"

#pragma warning(disable: 4251)
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
