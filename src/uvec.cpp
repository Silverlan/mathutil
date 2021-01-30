/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mathutil/uvec.h"
#include "mathutil/umath_frustum.hpp"
#include <sharedutils/util_string.h>
#include <glm/gtx/projection.hpp>

using namespace umath;

static Vector3 VECTOR_AXIS_X(1.f,0.f,0.f);
static Vector3 VERTOR_AXIS_Y(0.f,1.f,0.f);
static Vector3 VECTOR_AXIS_Z(0.f,0.f,1.f);

Vector3 uvec::create(const std::string &str)
{
	Vector3 r;
	ustring::string_to_array<glm::vec3::value_type,Double>(str,&r[0],atof,3);
	return r;
}

Vector4 uvec::create_v4(const std::string &str)
{
	Vector4 r;
	ustring::string_to_array<glm::vec4::value_type,Double>(str,&r[0],atof,4);
	return r;
}

Vector2 uvec::create_random_unit_vector2()
{
	Float azimuth = umath::random(0.f,2.f) *Float(M_PI);
	return Vector2(std::cos(azimuth),std::sin(azimuth));
}

Vector3 uvec::create_random_unit_vector()
{
	Float azimuth = umath::random(0.f,2.f) *Float(M_PI);
	Vector2 planar(std::cos(azimuth),std::sin(azimuth));

	Float z = umath::random(-1.f,1.f);
	planar *= std::sqrt(1.f -z *z);
	return Vector3(planar.x,planar.y,z);
}

Vector3 uvec::get_random_spread(const Vector3 &a,const Vector3 &b,Float)
{
	Vector3 c = (a +b) /2.f;
	Float r = sqrtf(powf(glm::distance(a,b) /2.f,2));
	Float x = umath::random(-r,r);
	Float rSq = powf(r,2);
	Float xSq = powf(x,2);
	Float t = rSq -xSq;
	Float limY = (t > 0 ? sqrtf(t) : t);
	Float y = umath::random(-limY,limY);
	t -= powf(y,2);
	Float limZ = (t > 0 ? sqrtf(t) : t);
	Float z = umath::random(-limZ,limZ);
	return c +Vector3(x,y,z);
}

Vector3 uvec::create(Float x,Float y,Float z) {return Vector3(x,y,z);}

Vector3 uvec::create(Vector3 &vec) {return vec;}

void uvec::mul(Vector3 *vec,Float f)
{
	vec->x *= f;
	vec->y *= f;
	vec->z *= f;
}

void uvec::mul(Vector3 *vec,Int32 f)
{
	vec->x *= f;
	vec->y *= f;
	vec->z *= f;
}

void uvec::mul(Vector3 *vec,Double f)
{
	vec->x *= static_cast<Float>(f);
	vec->y *= static_cast<Float>(f);
	vec->z *= static_cast<Float>(f);
}

void uvec::div(Vector3 *vec,Float f)
{
	vec->x /= f;
	vec->y /= f;
	vec->z /= f;
}

void uvec::div(Vector3 *vec,Int32 f)
{
	vec->x /= f;
	vec->y /= f;
	vec->z /= f;
}

void uvec::div(Vector3 *vec,Double f)
{
	vec->x /= static_cast<Float>(f);
	vec->y /= static_cast<Float>(f);
	vec->z /= static_cast<Float>(f);
}

void uvec::add(Vector3 *vecA,const Vector3 &vecB)
{
	vecA->x += vecB.x;
	vecA->y += vecB.y;
	vecA->z += vecB.z;
}

void uvec::sub(Vector3 *vecA,const Vector3 &vecB)
{
	vecA->x -= vecB.x;
	vecA->y -= vecB.y;
	vecA->z -= vecB.z;
}

void uvec::min(Vector3 *vecA,const Vector3 &vecB)
{
	if(vecB.x < vecA->x) vecA->x = vecB.x;
	if(vecB.y < vecA->y) vecA->y = vecB.y;
	if(vecB.z < vecA->z) vecA->z = vecB.z;
}

void uvec::max(Vector3 *vecA,const Vector3 &vecB)
{
	if(vecB.x > vecA->x) vecA->x = vecB.x;
	if(vecB.y > vecA->y) vecA->y = vecB.y;
	if(vecB.z > vecA->z) vecA->z = vecB.z;
}

Vector3 uvec::max() {return Vector3{std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::max()};}
Vector3 uvec::min() {return Vector3{std::numeric_limits<float>::lowest(),std::numeric_limits<float>::lowest(),std::numeric_limits<float>::lowest()};}

void uvec::zero(Vector3 *vec)
{
	vec->x = 0;
	vec->y = 0;
	vec->z = 0;
}

void uvec::match(Vector3 *vecA,const Vector3 &vecB)
{
	vecA->x = vecB.x;
	vecA->y = vecB.y;
	vecA->z = vecB.z;
}

void uvec::min(Vector3 *vec)
{
	vec->x = std::numeric_limits<glm::vec3::value_type>::lowest();
	vec->y = std::numeric_limits<glm::vec3::value_type>::lowest();
	vec->z = std::numeric_limits<glm::vec3::value_type>::lowest();
}

void uvec::max(Vector3 *vec)
{
	vec->x = std::numeric_limits<glm::vec3::value_type>::max();
	vec->y = std::numeric_limits<glm::vec3::value_type>::max();
	vec->z = std::numeric_limits<glm::vec3::value_type>::max();
}

EulerAngles uvec::to_angle(const Vector3 &vec)
{
	auto ang = EulerAngles(vec);
	ang.r = 0.f;
	if(umath::abs(ang.p) > 90.f) // Assume that yaw is the most important axis, so make sure yaw rotation
		ang.Flip();
	return ang;
}

EulerAngles uvec::to_angle(const Vector3 &vec,const Vector3 &up)
{
	auto ang = EulerAngles(vec,up);
	ang.r = 0.f;
	if(umath::abs(ang.p) > 90.f)
		ang.Flip();
	return ang;
}

Quat uvec::to_quaternion(const Vector3 &axis,Float angle)
{
	angle = static_cast<Float>(umath::deg_to_rad(angle)) *0.5f;
	Float sa = sinf(angle);
	Float ca = cosf(angle);
	return Quat(
		axis.x *sa,
		axis.y *sa,
		axis.z *sa,
		ca
	);
}

Float uvec::get_yaw(const Vector3 &v)
{
	return static_cast<Float>(umath::rad_to_deg(atan2f(v.x,v.z)));
}
Float uvec::get_pitch(const Vector3 &v)
{
	return 360.f -static_cast<Float>(umath::rad_to_deg(atan2f(v.y,sqrtf((v.x *v.x) +(v.z *v.z)))));
}
Float uvec::dot(const Vector3 &a,const Vector3 &b)
{
	return glm::dot(a,b);
}

void uvec::normalize(Vector3 *x)
{
	/*glm::detail::tvec3<glm::mediump_float>::value_type sqr = x->x *x->x +x->y *x->y +x->z *x->z;
	if(sqr == 0.0)
		return;
	*x = *x *glm::inversesqrt(sqr);*/
	*x = glm::normalize(*x);
}

Vector3 uvec::get_normal(Vector3 vec)
{
	uvec::normalize(&vec);
	return vec;
}

static const Vector3 vPitch(1,0,0);
static const Vector3 vYaw(0,1,0);
static const Vector3 vRoll(0,0,1);
void uvec::rotate(Vector3 *vec,const EulerAngles &ang)
{
	Vector3 rotated = glm::rotate(*vec,CFloat(umath::deg_to_rad(ang.r)),vRoll);
	rotated = glm::rotate(rotated,CFloat(umath::deg_to_rad(ang.p)),vPitch);
	rotated = glm::rotate(rotated,CFloat(umath::deg_to_rad(ang.y)),vYaw);
	vec->x = rotated.x;
	vec->y = rotated.y;
	vec->z = rotated.z;
}

void uvec::rotate(Vector3 *vec,const Quat &rot)
{
	*vec = glm::rotate(rot,*vec);
}

Vector3 uvec::cross(const Vector3 &vecA,const Vector3 &vecB) {return glm::cross(vecA,vecB);}

Vector3 uvec::lerp(const Vector3 &vecA,const Vector3 &vecB,Float factor) {return vecA +(vecB -vecA) *factor;}

void uvec::rotate_around(Vector3 *vec,const EulerAngles &ang,const Vector3 &center)
{
	Vector3 rot = *vec -center;
	rotate(&rot,ang);
	rot += center;
	vec->x = rot.x;
	vec->y = rot.y;
	vec->z = rot.z;
}

Float uvec::length(const Vector3 &vec)
{
	return umath::sqrt(length_sqr(vec));
}

Float uvec::length_sqr(const Vector3 &vec)
{
	return umath::pow2(vec.x) +umath::pow2(vec.y) +umath::pow2(vec.z);
}

std::string uvec::to_string(Vector3 *vec)
{
	std::string r = "(";
	r += std::to_string(vec->x);
	r += ",";
	r += std::to_string(vec->y);
	r += ",";
	r += std::to_string(vec->z);
	r += ")";
	return r;
}

umath::Degree uvec::get_angle(const Vector3 &a,const Vector3 &b)
{
	auto dot = uvec::dot(a,b);
	auto la = uvec::length(a);
	auto lb = uvec::length(b);
	return umath::rad_to_deg(umath::acos(dot /(la *lb)));
}

Quat uvec::get_rotation(const Vector3 &a,const Vector3 &b)
{
#if 0
	auto v1 = a;
	auto v2 = b;
	if(uvec::length_sqr(v1) > 1e-06)
		uvec::normalize(&v1);
	if(uvec::length_sqr(v2) > 1e-06)
		uvec::normalize(&v2);
	Vector3 cross = uvec::cross(v1,v2);
	Float dot = uvec::dot(v1,v2);
	if(dot > 0.999999f)
		return Quat(1,0,0,0);
	Float w = 1.f +dot;
	if(dot < -0.999999f)
	{
		Vector3 r = uvec::cross(VECTOR_AXIS_X,v1);
		if(uvec::length(r) < 0.000001f)
			r = uvec::cross(VECTOR_AXIS_Z,v1);
		uvec::normalize(&r);
		return uquat::create(r,Float(M_PI));
	}
#pragma message ("TODO: Why do we have to negate the 'w'-component?")
	return Quat(-w,cross.x,cross.y,cross.z);
#endif
	// Source: https://raw.githubusercontent.com/ehsan/ogre/master/OgreMain/include/OgreVector3.h
	constexpr Vector3 zeroVector {};
	constexpr Vector3 fallbackAxis = zeroVector;
    // Based on Stan Melax's article in Game Programming Gems
    Quat q;
    // Copy, since cannot modify local
    Vector3 v0 = a;
    Vector3 v1 = b;
	if(uvec::length_sqr(v0) > 1e-06)
		uvec::normalize(&v0);
	if(uvec::length_sqr(v1) > 1e-06)
		uvec::normalize(&v1);

    auto d = uvec::dot(v0,v1);
    // If dot == 1, vectors are the same
    if (d >= 1.0f)
		return uquat::identity();
	if (d < (1e-6f - 1.0f))
	{
		if (fallbackAxis != zeroVector)
		{
			// rotate 180 degrees about the fallback axis
			q = uquat::create(fallbackAxis,umath::pi);
		}
		else
		{
			// Generate an axis
			Vector3 axis = uvec::cross(Vector3{1.f,0.f,0.f},a);
			if (uvec::length_sqr(axis) < 1e-06 *1e-06) // pick another if colinear
				axis = uvec::cross(Vector3{0.f,1.f,0.f},a);
			uvec::normalize(&axis);
			q = uquat::create(axis,umath::pi);
		}
	}
	else
	{
        auto s = umath::sqrt( (1+d)*2 );
	    auto invs = 1 / s;

		Vector3 c = uvec::cross(v0,v1);

    	q.x = c.x * invs;
        q.y = c.y * invs;
        q.z = c.z * invs;
        q.w = s * 0.5f;
		uquat::normalize(q);
	}
    return q;
}

Float uvec::distance(const Vector3 &a,const Vector3 &b) {return glm::distance(a,b);}

void uvec::local_to_world(const Vector3 &localPos,const Quat &localRot,Vector3 &ioPos,Quat &ioRot)
{
	local_to_world(localPos,localRot,ioPos);
	local_to_world(localRot,ioRot);
}
void uvec::local_to_world(const Vector3 &localPos,const Quat &localRot,Vector3 &ioPos)
{
	rotate(&ioPos,localRot);
	ioPos += localPos;
}
void uvec::local_to_world(const Quat &localRot,Quat &ioRot)
{
	ioRot = localRot *ioRot;
}

void uvec::world_to_local(const Vector3 &localPos,const Quat &localRot,Vector3 &ioPos,Quat &ioRot)
{
	world_to_local(localPos,localRot,ioPos);
	world_to_local(localRot,ioRot);
}
void uvec::world_to_local(const Vector3 &localPos,const Quat &localRot,Vector3 &ioPos)
{
	ioPos -= localPos;
	rotate(&ioPos,uquat::get_inverse(localRot));
}
void uvec::world_to_local(const Quat &localRot,Quat &ioRot)
{
	ioRot = uquat::get_inverse(localRot) *ioRot;
}

void uvec::snap_to_grid(Vector3 &v,UInt32 gridSize)
{
	v.x = CFloat(umath::snap_to_grid(v.x,gridSize));
	v.y = CFloat(umath::snap_to_grid(v.y,gridSize));
	v.z = CFloat(umath::snap_to_grid(v.z,gridSize));
}

Vector3 uvec::calc_world_direction_from_2d_coordinates(const Vector3 &forward,const Vector3 &right,const Vector3 &up,Float fovRad,Float nearZ,Float farZ,Float aspectRatio,Float width,Float height,const Vector2 &uv)
{
	auto pos0 = umath::frustum::get_plane_point({},forward,right,up,fovRad,nearZ,aspectRatio,uv);
	auto pos1 = umath::frustum::get_plane_point({},forward,right,up,fovRad,farZ,aspectRatio,uv);
	auto dir = pos1 -pos0;
	normalize(&dir);
	return dir;
}
Vector2 uvec::calc_screenspace_uv_from_worldspace_position(const Vector3 &point,const Mat4 &viewProjection,float nearZ,float farZ,float &outDist)
{
	Vector4 tmp {point.x,point.y,point.z,1.f};
	tmp = viewProjection *tmp;

	Vector3 tmp3 {tmp.x,tmp.y,tmp.z};
	tmp3 /= tmp.w;

	outDist = depth_to_distance(tmp3.z,nearZ,farZ);
	return {
		(tmp3.x +1.0) *0.5,
		1.0 -((1.0 -tmp3.y) *0.5)
	};
}
Vector2 uvec::calc_screenspace_uv_from_worldspace_position(const Vector3 &point,const Mat4 &viewProjection)
{
	float dist;
	// We don't care about the distance here, so we'll just use arbitrary values for the nearZ/farZ
	return calc_screenspace_uv_from_worldspace_position(point,viewProjection,0.f,0.f,dist);
}
float uvec::calc_screenspace_distance_to_worldspace_position(const Vector3 &point,const Mat4 &viewProjection,float nearZ,float farZ)
{
	float dist;
	calc_screenspace_uv_from_worldspace_position(point,viewProjection,nearZ,farZ,dist);
	return dist;
}
float uvec::depth_to_distance(double depth,float nearZ,float farZ)
{
	return nearZ *farZ /(farZ +nearZ -depth *(farZ -nearZ));
}
Vector2 uvec::calc_screenspace_direction_from_worldspace_direction(const Vector3 &n,const Mat4 &viewProjection)
{
	auto v = calc_screenspace_uv_from_worldspace_position(n,viewProjection) -calc_screenspace_uv_from_worldspace_position({},viewProjection);
	v = glm::normalize(v);
	return v;
}

Vector3 uvec::get_perpendicular(const Vector3 &v)
{
	assert(length(v) <= 0.0001f);
	auto dirPerp = v;
	for(uint8_t i=0;i<3;++i)
	{
		if(dirPerp[i] != 0.f)
		{
			dirPerp[i] = -dirPerp[i];
			if(umath::abs(umath::abs(dirPerp[i]) -1.f) <= 0.0001f)
				dirPerp[(i +1) %3] = 1.f;
			break;
		}
	}
	auto r = uvec::cross(v,dirPerp);
	uvec::normalize(&r);
	return r;
}

Vector3 uvec::project(const Vector3 &p,const Vector3 &n) {return glm::proj(p,n);}
Vector3 uvec::project_to_plane(const Vector3 &p,const Vector3 &n,float d) {return p -(dot(p,n) -d) *n;}
Mat3 uvec::calc_outer_product(const Vector3 &v0,const Vector3 &v1)
{
	return Mat3(
		v0.x *v1.x,v0.x *v1.y,v0.x *v1.z,
		v0.y *v1.x,v0.y *v1.y,v0.y *v1.z,
		v0.z *v1.x,v0.z *v1.y,v0.z *v1.z
	);
}
Vector3 uvec::calc_average(const std::vector<Vector3> &points)
{
	Vector3 avg{};
	for(auto &p : points)
		avg += p;
	return avg /static_cast<float>(points.size());
}

Float uvec::distance_sqr(const Vector3 &p0,const Vector3 &p1) {return length_sqr(p1 -p0);}

Float uvec::planar_distance_sqr(const Vector3 &p0,const Vector3 &p1,const Vector3 &n)
{
	auto p0n = project_to_plane(p0,n,0.f);
	auto p1n = project_to_plane(p1,n,0.f);
	return distance_sqr(p0n,p1n);
}

Float uvec::planar_distance(const Vector3 &p0,const Vector3 &p1,const Vector3 &n) {return sqrtf(planar_distance_sqr(p0,p1,n));}

double uvec::calc_area_of_triangle(const Vector3 &a,const Vector3 &b,const Vector3 &c)
{
	auto ab = b -a;
	auto ac = c -a;
	auto cross = uvec::cross(ab,ac);
	return 0.5 *uvec::length(cross);
}

Vector3 uvec::calc_point_on_triangle(const Vector3 &v0,const Vector3 &v1,const Vector3 &v2,float a,float b)
{
	return (1.f - umath::sqrt(a)) *v0 +(umath::sqrt(a) *(1.f -b)) *v1 +(umath::sqrt(a) *b) *v2;
}

Vector3 uvec::calc_face_normal(const Vector3 &v0,const Vector3 &v1,const Vector3 &v2)
{
	if(uvec::distance_sqr(v1 -v0,v2 -v0) < 0.001)
		return uvec::FORWARD;
	auto n = uvec::cross(v1 -v0,v2 -v0);
	uvec::normalize(&n);
	return n;
}

void uvec::calc_plane(const Vector3 &a,const Vector3 &b,const Vector3 &c,Vector3 &outPlaneNormal,float &outPlaneDistance)
{
	outPlaneNormal = glm::cross(b -a,c -a);
	uvec::normalize(&outPlaneNormal);
	outPlaneDistance = glm::dot(-outPlaneNormal,a);
}

std::ostream& operator<<(std::ostream &os,const Vector4 &vec)
{
	os<<vec.x<<" "<<vec.y<<" "<<vec.z<<" "<<vec.w;
	return os;
}

bool operator==(const Vector4 &a,const Vector4 &b) {return a.w == b.w && a.x == b.x && a.y == b.y && a.z == b.z;}

std::ostream& operator<<(std::ostream &os,const Vector3 &vec)
{
	os<<vec.x<<" "<<vec.y<<" "<<vec.z;
	return os;
}

bool operator==(const Vector3 &a,const Vector3 &b) {return a.x == b.x && a.y == b.y && a.z == b.z;}

std::ostream& operator<<(std::ostream &os,const Vector2 &vec)
{
	os<<vec.x<<" "<<vec.y;
	return os;
}

bool operator==(const Vector2 &a,const Vector2 &b) {return a.x == b.x && a.y == b.y;}

std::ostream& operator<<(std::ostream &os,const Vector2i &vec)
{
	os<<vec.x<<" "<<vec.y;
	return os;
}
bool operator==(const Vector2i &a,const Vector2i &b) {return a.x == b.x && a.y == b.y;}

std::ostream& operator<<(std::ostream &os,const Vector3i &vec)
{
	os<<vec.x<<" "<<vec.y<<" "<<vec.z;
	return os;
}
bool operator==(const Vector3i &a,const Vector3i &b) {return a.x == b.x && a.y == b.y && a.z == b.z;}
std::ostream& operator<<(std::ostream &os,const Vector4i &vec)
{
	os<<vec.x<<" "<<vec.y<<" "<<vec.z<<" "<<vec.w;
	return os;
}
bool operator==(const Vector4i &a,const Vector4i &b) {return a.w == b.w && a.x == b.x && a.y == b.y && a.z == b.z;}
