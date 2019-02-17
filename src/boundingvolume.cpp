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
