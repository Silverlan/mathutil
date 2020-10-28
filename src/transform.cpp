/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/.
*
* Copyright (c) 2020 Florian Weischer
*/

#include "mathutil/transform.hpp"
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

umath::Transform::Transform()
{
	SetIdentity();
}

umath::Transform::Transform(const Mat4 &t)
{
	umat::decompose(t,m_translation,m_rotation);
}

umath::Transform::Transform(const Vector3 &translation,const Quat &rotation)
	: m_translation{translation},m_rotation{rotation}
{}

umath::Transform umath::Transform::GetInverse() const
{
	Transform result {-m_translation,uquat::get_inverse(m_rotation)};
	uvec::rotate(&result.m_translation,result.m_rotation);
	return result;
}

const Vector3 &umath::Transform::GetOrigin() const {return const_cast<Transform*>(this)->GetOrigin();}
const Quat &umath::Transform::GetRotation() const {return const_cast<Transform*>(this)->GetRotation();}
Vector3 &umath::Transform::GetOrigin() {return m_translation;}
Quat &umath::Transform::GetRotation() {return m_rotation;}

Mat4 umath::Transform::ToMatrix() const
{
	auto m = glm::translate(Mat4{1.f},m_translation);
	m *= umat::create(m_rotation);
	return m;
}

void umath::Transform::SetOrigin(const Vector3 &origin) {m_translation = origin;}

void umath::Transform::SetRotation(const Quat &rot) {m_rotation = rot;}

void umath::Transform::SetIdentity()
{
	m_translation = {};
	m_rotation = uquat::identity();
}
void umath::Transform::TranslateGlobal(const Vector3 &v) {m_translation += v;}
void umath::Transform::TranslateLocal(const Vector3 &v)
{
	auto vrot = v;
	uvec::rotate(&vrot,m_rotation);
	m_translation += vrot;
}
void umath::Transform::RotateGlobal(const Quat &rot)
{
	uvec::rotate(&m_translation,rot);
	m_rotation = rot *m_rotation;
}
void umath::Transform::RotateLocal(const Quat &rot) {m_rotation *= rot;}
void umath::Transform::Interpolate(const Transform &dst,float factor)
{
	m_translation = uvec::lerp(m_translation,dst.m_translation,factor);
	m_rotation = uquat::slerp(m_rotation,dst.m_rotation,factor);
}
void umath::Transform::InterpolateToIdentity(float factor) {Interpolate({},factor);}
umath::Transform umath::Transform::operator*(const Transform &tOther) const
{
	auto res = *this;
	res *= tOther;
	return res;
}

umath::Transform &umath::Transform::operator*=(const Transform &tOther)
{
	auto translation = tOther.m_translation;
	uvec::rotate(&translation,m_rotation);
	m_rotation *= tOther.m_rotation;
	m_translation += translation;
	return *this;
}
Vector3 umath::Transform::operator*(const Vector3 &translation) const
{
	auto result = translation;
	uvec::rotate(&result,m_rotation);
	result += m_translation;
	return result;
}
Quat umath::Transform::operator*(const Quat &rot) const
{
	return m_rotation *rot;
}
umath::Transform umath::Transform::operator*(float weight) const
{
	auto res = *this;
	res *= weight;
	return res;
}
umath::Transform &umath::Transform::operator*=(float weight)
{
	m_translation *= weight;
	// Not entirely sure about this...
	m_rotation = uquat::slerp(uquat::identity(),m_rotation,weight);
	return *this;
}

/////////////

umath::ScaledTransform::ScaledTransform(const Transform &t)
	: Transform{t}
{}
umath::ScaledTransform::ScaledTransform(const Vector3 &pos,const Quat &rot,const Vector3 &scale)
	: Transform{pos,rot},m_scale{scale}
{}
void umath::ScaledTransform::SetIdentity()
{
	Transform::SetIdentity();
	m_scale = {1.f,1.f,1.f};
}
const Vector3 &umath::ScaledTransform::GetScale() const {return const_cast<ScaledTransform*>(this)->GetScale();}
Vector3 &umath::ScaledTransform::GetScale() {return m_scale;}
void umath::ScaledTransform::SetScale(const Vector3 &scale) {m_scale = scale;}
void umath::ScaledTransform::Scale(const Vector3 &scale) {m_scale *= scale;}
void umath::ScaledTransform::Interpolate(const ScaledTransform &dst,float factor)
{
	Transform::Interpolate(dst,factor);
	m_scale = uvec::lerp(m_scale,dst.m_scale,factor);
}
void umath::ScaledTransform::InterpolateToIdentity(float factor) {Interpolate({},factor);}
umath::ScaledTransform umath::ScaledTransform::GetInverse() const
{
	auto &scale = GetScale();
	ScaledTransform inverse {Transform::GetInverse()};
	inverse.SetScale(Vector3{1.f /scale.x,1.f /scale.y,1.f /scale.z}); // TODO: What to do about division by zero cases?
	return inverse;
}
umath::ScaledTransform umath::ScaledTransform::operator*(const ScaledTransform &tOther) const
{
	auto res = *this;
	res *= tOther;
	return res;
}
umath::ScaledTransform umath::ScaledTransform::operator*(const Transform &tOther) const
{
	auto res = *this;
	res *= tOther;
	return res;
}
umath::ScaledTransform &umath::ScaledTransform::operator*=(const ScaledTransform &tOther)
{
	Transform::operator*=(tOther);
	m_scale *= tOther.m_scale;
	return *this;
}
umath::ScaledTransform &umath::ScaledTransform::operator*=(const Transform &tOther)
{
	Transform::operator*=(tOther);
	return *this;
}
Vector3 umath::ScaledTransform::operator*(const Vector3 &translation) const
{
	return Transform::operator*(translation);
}
Quat umath::ScaledTransform::operator*(const Quat &rot) const
{
	return Transform::operator*(rot);
}

Mat4 umath::ScaledTransform::ToMatrix() const
{
	return Transform::ToMatrix() *glm::scale(glm::mat4{1.f},m_scale);
}

umath::ScaledTransform umath::ScaledTransform::operator*(float weight) const
{
	auto res = *this;
	res *= weight;
	return res;
}
umath::ScaledTransform &umath::ScaledTransform::operator*=(float weight)
{
	Transform::operator*=(weight);
	m_scale *= weight;
	return *this;
}

/////////////

Vector3 operator*(const Vector3 &v,const umath::Transform &t)
{
	return (umath::Transform{v,uquat::identity()} *t).GetOrigin();
}
Vector3 &operator*=(Vector3 &v,const umath::Transform &t)
{
	v = operator*(v,t);
	return v;
}
Quat operator*(const Quat &v,const umath::Transform &t)
{
	return (umath::Transform{Vector3{},v} *t).GetOrigin();
}
Quat &operator*=(Quat &v,const umath::Transform &t)
{
	v = operator*(v,t);
	return v;
}
umath::Transform operator*(float weight,const umath::Transform &t)
{
	return t *weight;
}

Vector3 operator*(const Vector3 &v,const umath::ScaledTransform &t)
{
	return operator*(v,static_cast<const umath::Transform&>(t));
}
Vector3 &operator*=(Vector3 &v,const umath::ScaledTransform &t)
{
	return operator*=(v,static_cast<const umath::Transform&>(t));
}
Quat operator*(const Quat &v,const umath::ScaledTransform &t)
{
	return operator*(v,static_cast<const umath::Transform&>(t));
}
Quat &operator*=(Quat &v,const umath::ScaledTransform &t)
{
	return operator*=(v,static_cast<const umath::Transform&>(t));
}
umath::ScaledTransform operator*(float weight,const umath::ScaledTransform &t)
{
	return t *weight;
}
