/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mathutil/umat.h"
#include "mathutil/uvec.h"

using namespace umath;

Mat4 umat::identity() {return glm::mat4(
	1.f,0.f,0.f,0.f,
	0.f,1.f,0.f,0.f,
	0.f,0.f,1.f,0.f,
	0.f,0.f,0.f,1.f
);}
Mat4 umat::create(const Quat &q) {return glm::toMat4(q);}

Mat4 umat::create_from_axis_angle(const Vector3 &v,Float ang)
{
	Float c = cosf(ang);
	Float s = sinf(ang);
	Float t = 1.0f -c;
	Mat4 m(1);
	m[0][0] = c +v.x *v.x *t;
	m[1][1] = c +v.y *v.y *t;
	m[2][2] = c +v.z *v.z *t;
	Float tmp1 = v.x *v.y *t;
	Float tmp2 = v.z *s;
	m[1][0] = tmp1 +tmp2;
	m[0][1] = tmp1 -tmp2;
	tmp1 = v.x *v.z *t;
	tmp2 = v.y *s;
	m[2][0] = tmp1 -tmp2;
	m[0][2] = tmp1 +tmp2;
	tmp1 = v.y *v.z *t;
	tmp2 = v.x *s;
	m[2][1] = tmp1 +tmp2;
	m[1][2] = tmp1 -tmp2;
	return m;
}

void umat::decompose(const Mat4 &t,Vector3 &outTranslation,Mat3 &outRotation,Vector3 *outScale)
{
	outTranslation = {t[3][0],t[3][1],t[3][2]};
	Vector3 scale {
		uvec::length(Vector3{t[0][0],t[0][1],t[0][2]}),
		uvec::length(Vector3{t[1][0],t[1][1],t[1][2]}),
		uvec::length(Vector3{t[2][0],t[2][1],t[2][2]})
	};
	if(outScale)
		*outScale = scale;
	outRotation = {
		t[0][0] /scale.x,t[1][0] /scale.y,t[2][0] /scale.z,
		t[0][1] /scale.x,t[1][1] /scale.y,t[2][1] /scale.z,
		t[0][2] /scale.x,t[1][2] /scale.y,t[2][2] /scale.z
	};
}
void umat::decompose(const Mat4 &t,Vector3 &outTranslation,Quat &outRotation,Vector3 *outScale)
{
	Mat3 mRot;
	decompose(t,outTranslation,mRot,outScale);
	outRotation = uquat::create(mRot);
}

Mat4 umat::look_at(const Vector3 &eye,const Vector3 &center,const Vector3 &up)
{
	Vector3 f = center -eye;
	uvec::normalize(&f);
	Vector3 u = uvec::get_normal(up);
	Vector3 s = glm::cross(f,u);
	if(s.x != 0 || s.y != 0 || s.z != 0)
		s = glm::normalize(s);
	u = glm::cross(s,f);

	Mat4 r(1);
	r[0][0] = s.x;
	r[1][0] = s.y;
	r[2][0] = s.z;
	r[0][1] = u.x;
	r[1][1] = u.y;
	r[2][1] = u.z;
	r[0][2] = -f.x;
	r[1][2] = -f.y;
	r[2][2] = -f.z;
	r[3][0] = -glm::dot(s,eye);
	r[3][1] = -glm::dot(u,eye);
	r[3][2] = glm::dot(f,eye);
	return r;
}

Mat3 umat::calc_covariance_matrix(const std::vector<Vector3> &points)
{
	auto avg = uvec::calc_average(points);
	return calc_covariance_matrix(points,avg);
}
Mat3 umat::calc_covariance_matrix(const std::vector<Vector3> &points,const Vector3 &avg)
{
	auto C = Mat3(0.f);
	for(auto p : points)
	{
		p -= avg;
		C += uvec::calc_outer_product(p,p);
	}
	return C;
}

Mat4 umat::create_from_axes(const Vector3 &forward,const Vector3 &right,const Vector3 &up)
{
	return Mat4(
		-right.x,-right.y,-right.z,0.f,
		up.x,up.y,up.z,0.f,
		forward.x,forward.y,forward.z,0.f,
		0.f,0.f,0.f,1.f
	);
}

void umat::to_axes(const Mat4 &m,Vector3 &outForward,Vector3 &outRight,Vector3 &outUp)
{
	outForward = {m[2][0],m[2][1],m[2][2]};
	outRight = {-m[0][0],-m[0][1],-m[0][2]};
	outUp = {m[1][0],m[1][1],m[1][2]};
}

float umat::calc_projection_depth_bias_offset(float p,float nearZ,float farZ,float d,float delta)
{
	// See http://mathfor3dgameprogramming.com/ , chapter 9.1.1
	// delta = Eye space z offset
	auto pz = -d; // Negative eye space depth value

	auto epsilon = -2.0 *farZ *nearZ *delta /((farZ +nearZ) *pz *(pz +delta));
	return p *(1.0 +epsilon);
}

void umat::apply_projection_depth_bias_offset(Mat4 &inOutP,float nearZ,float farZ,float d,float delta) {inOutP[2][2] = calc_projection_depth_bias_offset(inOutP[2][2],nearZ,farZ,d,delta);}

Vector2 umat::to_screen_uv(const Vector3 &point,const Mat4 &vp)
{
	Vector4 v{point,1.f};
	v = vp *v;
	return Vector2{
		((v.x /v.w) +1.f) /2.f,
		((v.y /v.w) +1.f) /2.f
	};
}
