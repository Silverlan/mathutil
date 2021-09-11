/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __QUATERNION_H__
#define __QUATERNION_H__
#include "mathutildefinitions.h"
#include "glmutil.h"
#include "umath.h"
#include "uvec.h"
#include "umat.h"

class EulerAngles;
namespace uquat
{
	static constexpr auto UNIT = Quat{};
	DLLMUTIL Quat create(const Vector3 &v,Float ang);
	DLLMUTIL Quat create(const EulerAngles &ang);
	DLLMUTIL Quat create(const Vector3 &forward,const Vector3 &right,const Vector3 &up);
	DLLMUTIL Quat create(const Mat4 &mat);
	DLLMUTIL Quat create(const Mat3 &rot);
	DLLMUTIL Quat create(const std::string &s);
	DLLMUTIL Vector3 forward(const Quat &q);
	DLLMUTIL Vector3 right(const Quat &q);
	DLLMUTIL Vector3 up(const Quat &q);
	DLLMUTIL Quat slerp(const Quat &q1,const Quat &q2,Float factor);
	DLLMUTIL Quat lerp(const Quat &q1,const Quat &q2,Float factor);
	DLLMUTIL void get_orientation(const Quat &q,Vector3 *forward,Vector3 *right,Vector3 *up);
	DLLMUTIL Float dot_product(const Quat &q1,const Quat &q2);
	DLLMUTIL void rotate(Quat &q,const EulerAngles &ang);
	DLLMUTIL void rotate(Quat &q,const Vector3 &axis,Float ang);
	DLLMUTIL Float length(const Quat &q);
	DLLMUTIL void normalize(Quat &q);
	DLLMUTIL void inverse(Quat &q);
	DLLMUTIL Quat get_inverse(const Quat &q);
	DLLMUTIL Quat get_normal(const Quat &q);
	DLLMUTIL void rotate_x(Quat &q,Float ang);
	DLLMUTIL void rotate_y(Quat &q,Float ang);
	DLLMUTIL void rotate_z(Quat &q,Float ang);
	DLLMUTIL bool cmp(const Quat &a,const Quat &b,float epsilon=0.0001f);
	DLLMUTIL Quat create_look_rotation(const Vector3 &forward,const Vector3 &up);
	DLLMUTIL float get_angle(const Quat &rot);
	DLLMUTIL void to_axis_angle(const Quat &rot,Vector3 &axis,float &angle);
	DLLMUTIL constexpr Quat identity() {return Quat{1.f,0.f,0.f,0.f};}
	DLLMUTIL Quat calc_average(const std::vector<Quat> &rotations);
	DLLMUTIL Quat clamp_rotation(const Quat &q,const EulerAngles &minBounds,const EulerAngles &maxBounds);
	DLLMUTIL float distance(const Quat &q0,const Quat &q1);
	DLLMUTIL std::string to_string(const Quat &q,char sep=',');
};

DLLMUTIL std::ostream &operator<<(std::ostream &out,const Quat &o);

#endif
