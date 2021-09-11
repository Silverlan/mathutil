/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __UVEC_H__
#define __UVEC_H__
#include "mathutildefinitions.h"
#include <string>

#include "glmutil.h"
#include <vector>
#include <iostream>
#include "umath.h"
#include "eulerangles.h"
#include "uquat.h"

DLLMUTIL std::ostream& operator<<(std::ostream &os,const Vector4 &vec);
DLLMUTIL bool operator==(const Vector4 &a,const Vector4 &b);
DLLMUTIL std::ostream& operator<<(std::ostream &os,const Vector3 &vec);
DLLMUTIL bool operator==(const Vector3 &a,const Vector3 &b);
DLLMUTIL std::ostream& operator<<(std::ostream &os,const Vector2 &vec);
DLLMUTIL bool operator==(const Vector2 &a,const Vector2 &b);
DLLMUTIL std::ostream& operator<<(std::ostream &os,const Vector2i &vec);
DLLMUTIL bool operator==(const Vector2i &a,const Vector2i &b);
DLLMUTIL std::ostream& operator<<(std::ostream &os,const Vector3i &vec);
DLLMUTIL bool operator==(const Vector3i &a,const Vector3i &b);
DLLMUTIL std::ostream& operator<<(std::ostream &os,const Vector4i &vec);
DLLMUTIL bool operator==(const Vector4i &a,const Vector4i &b);

#undef max
#undef min
namespace uvec
{
	DLLMUTIL Vector3 create(const std::string &str);
	DLLMUTIL Vector4 create_v4(const std::string &str);
	DLLMUTIL void mul(Vector3 *vec,Float f);
	DLLMUTIL void mul(Vector3 *vec,Int32 f);
	DLLMUTIL void mul(Vector3 *vec,Double f);

	DLLMUTIL void div(Vector3 *vec,Float f);
	DLLMUTIL void div(Vector3 *vec,Int32 f);
	DLLMUTIL void div(Vector3 *vec,Double f);

	DLLMUTIL void add(Vector3 *vecA,const Vector3 &vecB);
	DLLMUTIL void sub(Vector3 *vecA,const Vector3 &vecB);

	DLLMUTIL void min(Vector3 *vecA,const Vector3 &vecB);
	DLLMUTIL void max(Vector3 *vecA,const Vector3 &vecB);

	DLLMUTIL Vector3 max();
	DLLMUTIL Vector3 min();

	DLLMUTIL Vector3 create(Float x,Float y,Float z);
	DLLMUTIL Vector3 create(Vector3 &vec);
	DLLMUTIL Vector3 create_random_unit_vector();
	DLLMUTIL Vector2 create_random_unit_vector2();
	DLLMUTIL Vector3 get_random_spread(const Vector3 &a,const Vector3 &b,Float am=1.f);

	DLLMUTIL void zero(Vector3 *vec);
	DLLMUTIL void match(Vector3 *vecA,const Vector3 &vecB);
	DLLMUTIL void min(Vector3 *vec);
	DLLMUTIL void max(Vector3 *vec);

	DLLMUTIL std::string to_string(Vector3 *vec);

	DLLMUTIL EulerAngles to_angle(const Vector3 &vec);
	DLLMUTIL EulerAngles to_angle(const Vector3 &vec,const Vector3 &up);
	DLLMUTIL Quat to_quaternion(const Vector3 &axis,Float angle);
	DLLMUTIL Float get_yaw(const Vector3 &v);
	DLLMUTIL Float get_pitch(const Vector3 &v);
	DLLMUTIL Float dot(const Vector3 &a,const Vector3 &b);

	DLLMUTIL void normalize(Vector3 *vec);
	DLLMUTIL Vector3 get_normal(Vector3 vec);

	DLLMUTIL void rotate(Vector3 *vec,const EulerAngles &ang);
	DLLMUTIL void rotate(Vector3 *vec,const Quat &rot);
	DLLMUTIL void rotate_around(Vector3 *vec,const EulerAngles &ang,const Vector3 &center);

	DLLMUTIL Float length(const Vector3 &vec);
	DLLMUTIL Float length_sqr(const Vector3 &vec);

	DLLMUTIL Vector3 lerp(const Vector3 &vecA,const Vector3 &vecB,Float factor=0.5);
	DLLMUTIL Vector3 cross(const Vector3 &vecA,const Vector3 &vecB);

	// Requires two normalized vectors and returns the rotation between them. The result is NOT normalized.
	DLLMUTIL Quat get_rotation(const Vector3 &v1,const Vector3 &v2);

	DLLMUTIL umath::Degree get_angle(const Vector3 &v1,const Vector3 &v2);

	DLLMUTIL Float distance(const Vector3 &a,const Vector3 &b);

	DLLMUTIL void local_to_world(const Vector3 &localPos,const Quat &localRot,Vector3 &ioPos,Quat &ioRot);
	DLLMUTIL void local_to_world(const Vector3 &localPos,const Quat &localRot,Vector3 &ioPos);
	DLLMUTIL void local_to_world(const Quat &localRot,Quat &ioRot);

	DLLMUTIL void world_to_local(const Vector3 &localPos,const Quat &localRot,Vector3 &ioPos,Quat &ioRot);
	DLLMUTIL void world_to_local(const Vector3 &localPos,const Quat &localRot,Vector3 &ioPos);
	DLLMUTIL void world_to_local(const Quat &localRot,Quat &ioRot);

	DLLMUTIL void snap_to_grid(Vector3 &v,UInt32 gridSize=1);

	DLLMUTIL Vector3 calc_world_direction_from_2d_coordinates(const Vector3 &forward,const Vector3 &right,const Vector3 &up,Float fovRad,Float nearZ,Float farZ,Float aspectRatio,Float width,Float height,const Vector2 &uv);
	DLLMUTIL Vector2 calc_screenspace_uv_from_worldspace_position(const Vector3 &point,const Mat4 &viewProjection,float nearZ,float farZ,float &outDist);
	DLLMUTIL Vector2 calc_screenspace_uv_from_worldspace_position(const Vector3 &point,const Mat4 &viewProjection);
	DLLMUTIL Vector2 calc_screenspace_direction_from_worldspace_direction(const Vector3 &n,const Mat4 &viewProjection);
	DLLMUTIL float calc_screenspace_distance_to_worldspace_position(const Vector3 &point,const Mat4 &viewProjection,float nearZ,float farZ);
	DLLMUTIL float depth_to_distance(double depth,float nearZ,float farZ);

	static const auto FORWARD = Vector3(0,0,1);
	static const auto UP = Vector3(0,1,0);
	static const auto RIGHT = Vector3(-1,0,0);
	static const auto ORIGIN = Vector3(0.f,0.f,0.f);
	static const auto MIN = Vector3(std::numeric_limits<decltype(Vector3::x)>::lowest(),std::numeric_limits<decltype(Vector3::x)>::lowest(),std::numeric_limits<decltype(Vector3::x)>::lowest());
	static const auto MAX = Vector3(std::numeric_limits<decltype(Vector3::x)>::max(),std::numeric_limits<decltype(Vector3::x)>::max(),std::numeric_limits<decltype(Vector3::x)>::max());

	DLLMUTIL Vector3 get_perpendicular(const Vector3 &v);

	template<class T>
		bool cmp(const T &a,const T &b,const T &epsilon=T{0.0001});

	DLLMUTIL Vector3 project(const Vector3 &p,const Vector3 &n);
	DLLMUTIL Vector3 project_to_plane(const Vector3 &p,const Vector3 &n,float d);
	DLLMUTIL Mat3 calc_outer_product(const Vector3 &v0,const Vector3 &v1);
	DLLMUTIL Vector3 calc_average(const std::vector<Vector3> &points);
	template<class TVec>
		void to_min_max(TVec &min,TVec &max);
	template<class TVec>
		void to_min_max(TVec &min,TVec &max,const TVec &minOther,const TVec &maxOther);
	DLLMUTIL Float distance_sqr(const Vector3 &p0,const Vector3 &p1);
	DLLMUTIL Float planar_distance_sqr(const Vector3 &p0,const Vector3 &p1,const Vector3 &n);
	DLLMUTIL Float planar_distance(const Vector3 &p0,const Vector3 &p1,const Vector3 &n);

	DLLMUTIL void calc_plane(const Vector3 &a,const Vector3 &b,const Vector3 &c,Vector3 &outPlaneNormal,float &outPlaneDistance);

	DLLMUTIL double calc_area_of_triangle(const Vector3 &v0,const Vector3 &v1,const Vector3 &v2);
	DLLMUTIL Vector3 calc_point_on_triangle(const Vector3 &v0,const Vector3 &v1,const Vector3 &v2,float a,float b);
	DLLMUTIL Vector3 calc_face_normal(const Vector3 &v0,const Vector3 &v1,const Vector3 &v2);

	DLLMUTIL constexpr Vector3 xyz(const Vector4 &v)
	{
		return {v.x,v.y,v.z};
	}

	DLLMUTIL constexpr Vector3 vec3(float v)
	{
		return {v,v,v};
	}

	template<typename T> requires(umath::is_vector_type<T>)
		std::string to_string(const T &v,char sep=',')
	{
		std::string s;
		constexpr auto len = T::length();
		for(auto i=decltype(len){0u};i<len;++i)
		{
			if(i > 0)
				s += sep;
			s += std::to_string(v[i]);
		}
		return s;
	}
};

DLLMUTIL constexpr Vector3 operator+(const Vector3 &v,float f)
{
	return {
		v.x +f,
		v.y +f,
		v.z +f
	};
}

DLLMUTIL constexpr Vector3 operator-(const Vector3 &v,float f)
{
	return {
		v.x -f,
		v.y -f,
		v.z -f
	};
}

DLLMUTIL constexpr Vector3 operator/(float f,const Vector3 &v)
{
	return {
		f /v.x,
		f /v.y,
		f /v.z
	};
}

DLLMUTIL constexpr Vector3 operator+(float f,const Vector3 &v)
{
	return {
		f +v.x,
		f +v.y,
		f +v.z
	};
}

DLLMUTIL constexpr Vector3 operator-(float f,const Vector3 &v)
{
	return {
		f -v.x,
		f -v.y,
		f -v.z
	};
}

template<class T>
	bool uvec::cmp(const T &a,const T &b,const T &epsilon)
{
	return glm::all(glm::lessThan(glm::abs(a -b),epsilon));
}

template<class TVec>
	void uvec::to_min_max(TVec &min,TVec &max)
{
	for(auto i=decltype(min.length()){0};i<min.length();++i)
	{
		if(max[i] < min[i])
		{
			auto tmp = min[i];
			min[i] = max[i];
			max[i] = tmp;
		}
	}
}

template<class TVec>
	void uvec::to_min_max(TVec &min,TVec &max,const TVec &minOther,const TVec &maxOther)
{
	uvec::min(&min,minOther);
	uvec::min(&min,maxOther);

	uvec::max(&max,minOther);
	uvec::max(&max,maxOther);
}

#endif // __VECTOR3_H__
