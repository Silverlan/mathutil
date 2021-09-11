/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mathutil/uquat.h"
#include "mathutil/umath.h"
#include "mathutil/uvec.h"
#include "mathutil/eulerangles.h"
#include <sharedutils/util_string.h>
#include <glm/gtx/euler_angles.hpp>

using namespace umath;

Quat uquat::calc_average(const std::vector<Quat> &rotations)
{
	if(rotations.empty())
		return identity();
	auto qAvg = rotations.front();
	float weight;
	for(auto i=decltype(rotations.size()){1};i<rotations.size();++i)
	{
		auto weight = 1.f /(i +1.f);
		qAvg = slerp(qAvg,rotations.at(i),weight);
	}
	return qAvg;
}

Quat uquat::clamp_rotation(const Quat &pq,const EulerAngles &minBounds,const EulerAngles &maxBounds)
{
	// Source: https://forum.unity.com/threads/how-do-i-clamp-a-quaternion.370041/#post-5494723
	auto q = pq;
	q.x /= q.w;
	q.y /= q.w;
	q.z /= q.w;
	q.w = 1.0;
 
	auto angleX = static_cast<float>(2.f *umath::rad_to_deg(atan(q.x)));
	angleX = umath::clamp(angleX, minBounds.p, maxBounds.p);
	q.x = umath::tan(0.5 *umath::deg_to_rad(angleX));
 
	auto angleY = static_cast<float>(2.f *umath::rad_to_deg(atan(q.y)));
	angleY = umath::clamp(angleY, minBounds.y, maxBounds.y);
	q.y = umath::tan(0.5 *umath::deg_to_rad(angleY));
 
	auto angleZ = static_cast<float>(2.f *umath::rad_to_deg(atan(q.z)));
	angleZ = umath::clamp(angleZ, minBounds.r, maxBounds.r);
	q.z = umath::tan(0.5 *umath::deg_to_rad(angleZ));
	normalize(q);
	return q;
}

Quat uquat::create(const Vector3 &forward,const Vector3 &right,const Vector3 &up) {return create(umat::create_from_axes(forward,right,up));}

Quat uquat::create(const Mat3 &rot)
{
	// http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
	auto w = umath::sqrt(1.f +rot[0][0] +rot[1][1] +rot[2][2]) /2.f;
	auto w4 = (4.f *w);
	auto x = (rot[2][1] - rot[1][2]) /w4;
	auto y = (rot[0][2] - rot[2][0]) /w4;
	auto z = (rot[1][0] - rot[0][1]) /w4;
	return {w,x,y,z};
}

Quat uquat::create(const Vector3 &v,Float ang)
{
	return glm::angleAxis(ang,v);
	/*Float sAng;
	ang *= 0.5f;
	Vector3 vn = uvec::get_normal(v);

	sAng = umath::sin(ang);
	return Quat(
		umath::cos(ang),
		vn.x *sAng,
		vn.y *sAng,
		vn.z *sAng
	);*/
}

Quat uquat::create(const EulerAngles &ang)
{
	return glm::quat_cast(glm::eulerAngleYXZ(umath::deg_to_rad(ang.y),umath::deg_to_rad(ang.p),umath::deg_to_rad(ang.r)));

	// Obsolete
	/*Float pitch = static_cast<Float>(umath::deg_to_rad(ang.p));
	Float yaw = static_cast<Float>(umath::deg_to_rad(ang.y));
	Float roll = static_cast<Float>(umath::deg_to_rad(ang.r));
	Float c1 = cosf(yaw /2.f);
	Float c2 = cosf(roll /2.f);
	Float c3 = cosf(pitch /2.f);
	Float s1 = sinf(yaw /2.f);
	Float s2 = sinf(roll /2.f);
	Float s3 = sinf(pitch /2.f);
	return Quat( // Rotation order: Yaw (Rotation around y), Pitch (Rotation around x), Roll (Rotation around z)
		c1 *c2 *c3 -s1 *s2 *s3,
		s1 *s2 *c3 +c1 *c2 *s3,
		s1 *c2 *c3 +c1 *s2 *s3,
		c1 *s2 *c3 -s1 *c2 *s3
	);*/
}

Quat uquat::create(const Mat4 &m)
{
	return Quat(m);
	/*Quat q;
	Float tr = m[0][0] +m[1][1] +m[2][2];
	if(tr > 0.f)
	{
		Float s = sqrtf(tr +1.0f) *2.f;
		q.w = 0.25f *s;
		q.x = (m[2][1] -m[1][2]) /s;
		q.y = (m[0][2] -m[2][0]) /s;
		q.z = (m[1][0] -m[0][1]) /s;
	}
	else if((m[0][0] > m[1][1]) && (m[0][0] > m[2][2]))
	{
		Float s = sqrtf(1.0f +m[0][0] -m[1][1] -m[2][2]) *2.0f;
		q.w = (m[2][1] -m[1][2]) /s;
		q.x = 0.25f *s;
		q.y = (m[0][1] +m[1][0]) /s;
		q.z = (m[0][2] +m[2][0]) /s;
	}
	else if(m[1][1] > m[2][2])
	{
		Float s = sqrtf(1.0f +m[1][1] -m[0][0] -m[2][2]) *2.0f;
		q.w = (m[0][2] -m[2][0]) /s;
		q.x = (m[0][1] +m[1][0]) /s;
		q.y = 0.25f *s;
		q.z = (m[1][2] +m[2][1]) /s;
	}
	else
	{
		Float s = sqrtf(1.0f +m[2][2] -m[0][0] -m[1][1]) *2.0f;
		q.w = (m[1][0] -m[0][1]) /s;
		q.x = (m[0][2] -m[2][0]) /s;
		q.y = (m[1][2] +m[2][1]) /s;
		q.z = 0.25f *s;
	}
	return q;*/
}

Quat uquat::create(const std::string &s)
{
	Quat r {};
	ustring::string_to_array<Float,Double>(s,&r.x,atof,4);
	return Quat{r.x,r.y,r.z,r.w};
}

bool uquat::cmp(const Quat &a,const Quat &b,float epsilon)
{
	return (
		glm::abs(a.x -b.x) <= epsilon &&
		glm::abs(a.y -b.y) <= epsilon &&
		glm::abs(a.z -b.z) <= epsilon &&
		glm::abs(a.w -b.w) <= epsilon
	) ? true : false;
}

Vector3 uquat::forward(const Quat &q)
{
	return Vector3(
		2 *(q.x *q.z +q.w *q.y),
		2 *(q.y *q.z -q.w *q.x),
		1 -2 *(q.x *q.x +q.y *q.y)
	);
}
Vector3 uquat::right(const Quat &q)
{
	return -Vector3(
		1 -2 *(q.y *q.y +q.z *q.z),
		2 *(q.x *q.y +q.w *q.z),
		2 *(q.x *q.z -q.w *q.y)
	);
}
Vector3 uquat::up(const Quat &q)
{
	return Vector3(
		2 *(q.x *q.y -q.w *q.z),
		1 -2 *(q.x *q.x +q.z *q.z),
		2 *(q.y *q.z +q.w *q.x)
	);
}

Quat uquat::slerp(const Quat &q1,const Quat &q2,Float factor) {return glm::slerp(q1,q2,factor);}
Quat uquat::lerp(const Quat &q1,const Quat &q2,Float factor) {return glm::lerp(q1,q2,factor);}
void uquat::get_orientation(const Quat &q,Vector3 *forward,Vector3 *right,Vector3 *up)
{
	if(forward != nullptr)
		*forward = uquat::forward(q);
	if(right != nullptr)
		*right = uquat::right(q);
	if(up != nullptr)
		*up = uquat::up(q);
}
Float uquat::dot_product(const Quat &q1,const Quat &q2) {return glm::dot(q1,q2);}
void uquat::rotate(Quat &q,const EulerAngles &ang)
{
	q = glm::rotate(q,ang.r,uvec::RIGHT);
	q = glm::rotate(q,ang.p,uvec::FORWARD);
	q = glm::rotate(q,ang.y,uvec::UP);
}
Float uquat::length(const Quat &q) {return umath::sqrt(pow(q.w,2) +pow(q.x,2) +pow(q.y,2) +pow(q.z,2));}
void uquat::normalize(Quat &q)
{
	Float magnitude = length(q);
	if(magnitude == 0.f)
	{
		q.w = 1.f;
		return;
	}
	q.w /= magnitude;
	q.x /= magnitude;
	q.y /= magnitude;
	q.z /= magnitude;
}
void uquat::inverse(Quat &q) {q = glm::inverse(q);}
Quat uquat::get_inverse(const Quat &q) {return glm::inverse(q);}
Quat uquat::get_normal(const Quat &q)
{
	auto r = q;
	normalize(r);
	return r;
}
void uquat::rotate_x(Quat &q,Float ang) {auto r = create(uvec::FORWARD,ang); q *= r;}
void uquat::rotate_y(Quat &q,Float ang) {auto r = create(uvec::UP,ang); q = r *q;}
void uquat::rotate_z(Quat &q,Float ang) {auto r = create(uvec::RIGHT,ang); q *= r;}
void uquat::rotate(Quat &q,const Vector3 &axis,Float ang) {q *= create(axis,ang);}

Quat uquat::create_look_rotation(const Vector3 &forward,const Vector3 &up)
{
	// Source: http://pastebin.com/ubATCxJY (http://www.gamefaqs.com/boards/210-game-design-and-programming/71121728)
	auto &vector = forward; // Assuming forward is normalized
	auto vector2 = uvec::cross(up,vector);
	uvec::normalize(&vector2);
	auto vector3 = uvec::cross(vector,vector2);
	auto m00 = vector2.x;
	auto m01 = vector2.y;
	auto m02 = vector2.z;
	auto m10 = vector3.x;
	auto m11 = vector3.y;
	auto m12 = vector3.z;
	auto m20 = vector.x;
	auto m21 = vector.y;
	auto m22 = vector.z;
 
	double num8 = (m00 +m11) +m22;
	Quat quaternion {};
	if(num8 > 0.0)
	{
		auto num = umath::sqrt(num8 +1.0);
		quaternion.w = static_cast<float>(num *0.5);
		num = 0.5 /num;
		quaternion.x = static_cast<float>((m12 -m21) *num);
		quaternion.y = static_cast<float>((m20 -m02) *num);
		quaternion.z = static_cast<float>((m01 -m10) *num);
		return quaternion;
	}
	if((m00 >= m11) && (m00 >= m22))
	{
		auto num7 = umath::sqrt(((1.0 +m00) -m11) -m22);
		auto num4 = 0.5 /num7;
		quaternion.x = static_cast<float>(0.5 *num7);
		quaternion.y = static_cast<float>((m01 +m10) *num4);
		quaternion.z = static_cast<float>((m02 +m20) *num4);
		quaternion.w = static_cast<float>((m12 -m21) *num4);
		return quaternion;
	}
	if(m11 > m22)
	{
		auto num6 = umath::sqrt(((1.0 +m11) -m00) -m22);
		auto num3 = 0.5 /num6;
		quaternion.x = static_cast<float>((m10 +m01) *num3);
		quaternion.y = static_cast<float>(0.5 *num6);
		quaternion.z = static_cast<float>((m21 +m12) *num3);
		quaternion.w = static_cast<float>((m20 -m02) *num3);
		return quaternion;
	}
	auto num5 = umath::sqrt(((1.0 +m22) -m00) -m11);
	auto num2 = 0.5 /num5;
	quaternion.x = static_cast<float>((m20 +m02) *num2);
	quaternion.y = static_cast<float>((m21 +m12) *num2);
	quaternion.z = static_cast<float>(0.5 *num5);
	quaternion.w = static_cast<float>((m01 -m10) *num2);
	return quaternion;
}

float uquat::get_angle(const Quat &rot)
{
	return 2.f *umath::acos(rot.w);
}

void uquat::to_axis_angle(const Quat &rot,Vector3 &axis,float &angle)
{
	auto w2 = rot.w *rot.w;
	if(umath::abs(1.f -w2) < 0.0001)
	{
		// It's a identity quaternion
		axis = uvec::FORWARD;
		angle = 0.f;
		return;
	}
	angle = get_angle(rot);
	axis.x = rot.x /umath::sqrt(1-w2);
	axis.y = rot.y /umath::sqrt(1-w2);
	axis.z = rot.z /umath::sqrt(1-w2);
}

Radian uquat::distance(const Quat &q0,const Quat &q1)
{
	// Source: https://stackoverflow.com/a/23263233/2482983
	auto qd = uquat::get_inverse(q0) *q1;
	auto angle = 2.f * umath::atan2(uvec::length(Vector3{qd.x,qd.y,qd.z}),qd.w);
	return angle;
}

std::string uquat::to_string(const Quat &q,char sep)
{
	return std::to_string(q.w) +sep +std::to_string(q.x) +sep +std::to_string(q.y) +sep +std::to_string(q.z);
}

std::ostream &operator<<(std::ostream &out,const Quat &o)
{
	out<<o.w<<" "<<o.x<<" "<<o.y<<" "<<o.z;
	return out;
}
