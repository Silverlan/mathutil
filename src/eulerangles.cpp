/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <cmath>
#include "mathutil/eulerangles.h"
#include "mathutil/uquat.h"
#include "mathutil/uvec.h"
#include <sharedutils/util_string.h>
#include <glm/gtx/euler_angles.hpp>

std::ostream& operator<<(std::ostream &os,const EulerAngles &ang)
{
	os<<ang.p<<" "<<ang.y<<" "<<ang.r;
	return os;
}

EulerAngles operator/(const EulerAngles &ang,Float f) {return EulerAngles(ang.p /f,ang.y /f,ang.r /f);}
EulerAngles operator*(const EulerAngles &ang,Float f) {return EulerAngles(ang.p *f,ang.y *f,ang.r *f);}
EulerAngles operator/=(const EulerAngles &ang,Float f) {return ang /f;}
EulerAngles operator*=(const EulerAngles &ang,Float f) {return ang *f;}
EulerAngles operator*(Float f,const EulerAngles &a) {return EulerAngles(f *a.p,f *a.y,f *a.r);}

EulerAngles operator+(const EulerAngles &a,const EulerAngles &b) {return EulerAngles(a.p +b.p,a.y +b.y,a.r +b.r);}
EulerAngles operator-(const EulerAngles &a,const EulerAngles &b) {return EulerAngles(a.p -b.p,a.y -b.y,a.r -b.r);}
EulerAngles operator-(const EulerAngles &a) {return EulerAngles(-a.p,-a.y,-a.r);}
EulerAngles operator+=(const EulerAngles &a,const EulerAngles &b) {return a +b;}
EulerAngles operator-=(const EulerAngles &a,const EulerAngles &b) {return a -b;}

EulerAngles::EulerAngles(const Mat4 &mat)
	: EulerAngles()
{
	Initialize(mat);
}
/*
static void fix_inverted_pole(EulerAngles &ang)
{
	ang.Normalize();
	if(ang.p < -90.f || ang.p > 90.f)
	{
		ang.y += 180.f;
		ang.r += 180.f;
		if(ang.p < -90.f)
			ang.p -= 180.f;
		else
			ang.p -= 540.f;
	}
}
*/
EulerAngles::EulerAngles(const Quat &rot)
	: EulerAngles()
{
	// Source: http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/
	auto sqw = umath::pow2(rot.w);
	auto sqx = umath::pow2(rot.x);
	auto sqy = umath::pow2(rot.y);
	auto sqz = umath::pow2(rot.z);
	auto unit = sqx +sqy +sqz +sqw; // if normalised is one, otherwise is correction factor
	auto t = rot.y *rot.z +(-rot.x) *rot.w;
	if(t > 0.499999f *unit)
	{
		y = umath::rad_to_deg(2.0 *umath::atan2(rot.z,rot.w));
		p = umath::rad_to_deg(-umath::pi /2.0);
		r = 0.f;
		return;
	}
	else if(t < -0.499999f *unit)
	{
		y = umath::rad_to_deg(-2.0 *umath::atan2(rot.z,rot.w));
		p = umath::rad_to_deg(umath::pi /2.0);
		r = 0.f;
		return;
	}

	// Source: http://bediyap.com/programming/convert-quaternion-to-euler-rotations/
	const auto &fThreeAxisRot = [](float unit,float r11,float r12,float r21,float r31,float r32,Vector3 &res) {
		res[0] = atan2(r31,r32);
		res[1] = asin(r21 /unit);
		res[2] = atan2(r11,r12);
	};

	Vector3 res;
	fThreeAxisRot(
		unit,
		2.f *(rot.x *rot.z +rot.w *rot.y),
		rot.w *rot.w -rot.x *rot.x -rot.y *rot.y +rot.z *rot.z,
		-2.f *(rot.y *rot.z -rot.w *rot.x),
		2.f *(rot.x *rot.y +rot.w *rot.z),
		rot.w *rot.w -rot.x *rot.x +rot.y *rot.y -rot.z *rot.z,
		res
	);
	p = umath::rad_to_deg(res.y);
	y = umath::rad_to_deg(res.z);
	r = umath::rad_to_deg(res.x);
	Normalize();
	if(umath::abs(r) >= umath::abs(umath::normalize_angle(r +180.f))) // Assume that roll is less important than pitch or yaw
		Flip();
	//if(p < -90.f || p > 90.f)
	/*if(umath::abs(r) >= 90.f || (umath::abs(p) >= 90.f && umath::abs(y) >= 90.f))
	{
		y += 180.f;
		r += 180.f;
		if(p < 0.f)
			p = 180.f -p;
		else
			p = -180.f -p;
	}
	Normalize();*/
}

static Quat create_quaternion(const Vector3 &forward)
{
	auto l = uvec::length(forward);
	if(l == 0.f) // Error, just return unit quaternion
		return Quat{};
	auto n = forward /l;
	return uquat::create_look_rotation(n,uvec::get_perpendicular(n));
}

EulerAngles::EulerAngles(const Vector3 &forward)
	: EulerAngles(create_quaternion(forward))
{
	r = 0.f;
	if(umath::abs(p) > 90.f) // Assume that yaw is the most important axis, so make sure yaw has shortest rotational path
		Flip();

	// Obsolete (Doesn't work)
	//this->p = 360.f -static_cast<Float>(umath::rad_to_deg(std::asin(Float(forward.y))));
	//this->y = static_cast<Float>(umath::rad_to_deg(std::atan2(Float(forward.x),Float(forward.z))));
	//this->r = 0.f;
	//fix_inverted_pole(*this);
}
EulerAngles::EulerAngles(const Vector3 &forward,const Vector3 &up)
	: EulerAngles(uquat::create_look_rotation(forward,up))
{
	r = 0.f;
	if(umath::abs(p) > 90.f) // Assume that yaw is the most important axis, so make sure yaw has shortest rotational path
		Flip();

	// TODO: Check me
	/*Vector3 left = glm::cross(up,forward);
	uvec::normalize(&left);

	Float xyDist = sqrtf(forward[0] *forward[0] +forward[2] *forward[2]);
	if(xyDist > 0.001f)
	{
		this->y = static_cast<Float>(umath::rad_to_deg(atan2f(forward[2],forward[0])));
		this->p = static_cast<Float>(umath::rad_to_deg(atan2f(-forward[1],xyDist)));

		Float up_z = (left[2] *forward[0]) -(left[0] *forward[2]);
		this->r = static_cast<Float>(umath::rad_to_deg(atan2f(left[1],up_z)));
	}
	else
	{
		this->y = static_cast<Float>(umath::rad_to_deg(atan2f(-left[0],left[2])));
		this->p = static_cast<Float>(umath::rad_to_deg(atan2f(-forward[1],xyDist)));
		this->r = 0.f;
	}*/
	//fix_inverted_pole(*this);
}
EulerAngles::EulerAngles(const std::string &str)
	: EulerAngles()
{
	ustring::string_to_array<Float,Double>(str,&p,atof,3);
}

void EulerAngles::Initialize(const Mat4 &mat)
{
	*this = EulerAngles(Quat(mat));

	// Obsolete
	/*Vector3 forward(mat[0][0],mat[1][0],mat[2][0]);
	Vector3 left(mat[0][1],mat[1][1],mat[2][1]);
	Vector3 up(0.f,0.f,mat[2][2]);
	Float xyDist = sqrtf(forward[0] *forward[0] +forward[1] *forward[1]);
	if(xyDist > 0.001f)
	{
		this->y = static_cast<Float>(umath::rad_to_deg(atan2f(forward[1],forward[0])));
		this->p = static_cast<Float>(umath::rad_to_deg(atan2f(-forward[2],xyDist)));
		this->r = static_cast<Float>(umath::rad_to_deg(atan2f(left[2],up[2])));
	}
	else
	{
		this->y = static_cast<Float>(umath::rad_to_deg(atan2f(-left[0],left[1])));
		this->p = static_cast<Float>(umath::rad_to_deg(atan2f(-forward[2],xyDist)));
		this->r = 0.f;
	}*/
}

void EulerAngles::Normalize(Float base)
{
	p = static_cast<float>(umath::normalize_angle(static_cast<double>(p),base));
	y = static_cast<float>(umath::normalize_angle(static_cast<double>(y),base));
	r = static_cast<float>(umath::normalize_angle(static_cast<double>(r),base));
}

void EulerAngles::Normalize()
{
	p = static_cast<float>(umath::normalize_angle(static_cast<double>(p)));
	y = static_cast<float>(umath::normalize_angle(static_cast<double>(y)));
	r = static_cast<float>(umath::normalize_angle(static_cast<double>(r)));
}

void EulerAngles::Flip()
{
	Normalize();
	y += 180.f;
	r += 180.f;
	if(p < 0.f)
		p = 180.f -p;
	else
		p = -180.f -p;
	Normalize();
}

Vector3 EulerAngles::Forward() const
{
	return uquat::forward(uquat::create(*this));
/*
#pragma message ("TODO: This doesn't take roll into account at the moment. FIXME")
	Float p = Math::DegToRad(this->p);
	Float y = Math::DegToRad(this->y);
	return Vector3(
		cos(p) *sin(y),
		-sin(p),
		cos(p) *cos(y)
	);
*/
}

Vector3 EulerAngles::Right() const
{
	return uquat::right(uquat::create(*this));
/*
#pragma message ("TODO: This doesn't take roll into account at the moment. FIXME")
	Float y = Math::DegToRad(this->y);
	return Vector3(
		sin(y -3.14f /2.0f),
		0,
		cos(y -3.14f /2.0f)
	);
*/
}

Vector3 EulerAngles::Up() const
{
	auto rot = uquat::create(*this);
	Vector3 forward = uquat::forward(rot);
	Vector3 right = uquat::right(rot);
	return Up(&forward,&right);
}

Vector3 EulerAngles::Up(Vector3 *forward,Vector3 *right) const {return glm::cross(*right,*forward);}

Float &EulerAngles::operator[](const uint8_t idx)
{
	assert(idx < 3);
	return (&p)[idx];
}
const Float &EulerAngles::operator[](const uint8_t idx) const
{
	assert(idx < 3);
	return (&p)[idx];
}
EulerAngles &EulerAngles::operator=(const EulerAngles &other)
{
	p = other.p;
	y = other.y;
	r = other.r;
	return *this;
}

void EulerAngles::GetOrientation(Vector3 *forward,Vector3 *right,Vector3 *up) const
{
	if(forward != nullptr)
		*forward = Forward();
	if(right != nullptr)
		*right = Right();
	if(up != nullptr)
	{
		Vector3 fwd = (forward != nullptr) ? *forward : Forward();
		Vector3 rgt = (right != nullptr) ? *right : Right();
		*up = Up(&fwd,&rgt);
	}
}

Mat4 EulerAngles::ToMatrix() const
{
	Mat4 mat(1.0f);
	mat = glm::rotate(mat,CFloat(umath::deg_to_rad(y)),uvec::UP);
	mat = glm::rotate(mat,CFloat(umath::deg_to_rad(p)),uvec::FORWARD);
	mat = glm::rotate(mat,CFloat(umath::deg_to_rad(r)),uvec::RIGHT);
	return mat;
}

void EulerAngles::Set(const EulerAngles &ang)
{
	this->p = ang.p;
	this->y = ang.y;
	this->r = ang.r;
}

EulerAngles EulerAngles::Copy() const {return EulerAngles(p,y,r);}
void EulerAngles::Approach(EulerAngles &ang,Float amount)
{
	p = static_cast<float>(umath::approach_angle(static_cast<double>(p),static_cast<double>(ang.p),static_cast<double>(amount)));
	y = static_cast<float>(umath::approach_angle(static_cast<double>(y),static_cast<double>(ang.y),static_cast<double>(amount)));
	r = static_cast<float>(umath::approach_angle(static_cast<double>(r),static_cast<double>(ang.r),static_cast<double>(amount)));
}
EulerAngles EulerAngles::Approach(EulerAngles &a,EulerAngles &b,Float amount)
{
	return EulerAngles(
		static_cast<float>(umath::approach_angle(static_cast<double>(a.p),static_cast<double>(b.p),static_cast<double>(amount))),
		static_cast<float>(umath::approach_angle(static_cast<double>(a.y),static_cast<double>(b.y),static_cast<double>(amount))),
		static_cast<float>(umath::approach_angle(static_cast<double>(a.r),static_cast<double>(b.r),static_cast<double>(amount)))
	);
}
