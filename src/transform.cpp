/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/.
*
* Copyright (c) 2021 Silverlan
*/

#include "mathutil/transform.hpp"
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

umath::Transform::Transform(const Mat4 &t) { umat::decompose(t, translation, rotation); }

umath::Transform::Transform(const Vector3 &translation, const Quat &rotation) : translation {translation}, rotation {rotation} {}

umath::Transform::Transform(const Vector3 &translation) : Transform {translation, uquat::identity()} {}
umath::Transform::Transform(const Quat &rotation) : Transform {Vector3 {}, rotation} {}

bool umath::Transform::operator==(const Transform &t) const { return uvec::cmp(translation, t.translation, {0.001f, 0.001f, 0.001f}) && uquat::cmp(rotation, t.rotation, 0.001f); }

umath::Transform umath::Transform::GetInverse() const
{
	Transform result {-translation, uquat::get_inverse(rotation)};
	uvec::rotate(&result.translation, result.rotation);
	return result;
}

umath::Plane umath::Transform::ToPlane() const
{
	auto &origin = GetOrigin();
	auto p0 = origin;
	auto p1 = origin + GetUp() * 10.f;
	auto p2 = origin + GetRight() * 10.f;
	return umath::Plane {p0, p1, p2};
}

EulerAngles umath::Transform::GetAngles() const { return EulerAngles {GetRotation()}; }
void umath::Transform::SetAngles(const EulerAngles &ang) { SetRotation(uquat::create(ang)); }
Vector3 umath::Transform::GetForward() const { return uquat::forward(GetRotation()); }
Vector3 umath::Transform::GetRight() const { return uquat::right(GetRotation()); }
Vector3 umath::Transform::GetUp() const { return uquat::up(GetRotation()); }

const Vector3 &umath::Transform::GetOrigin() const { return const_cast<Transform *>(this)->GetOrigin(); }
const Quat &umath::Transform::GetRotation() const { return const_cast<Transform *>(this)->GetRotation(); }
Vector3 &umath::Transform::GetOrigin() { return translation; }
Quat &umath::Transform::GetRotation() { return rotation; }

Mat4 umath::Transform::ToMatrix() const
{
	auto m = glm::translate(Mat4 {1.f}, translation);
	m *= umat::create(rotation);
	return m;
}

void umath::Transform::Reflect(const Vector3 &n, float d)
{
	auto &rot = GetRotation();
	Vector3 forward, right, up;
	uquat::get_orientation(rot, &forward, &right, &up);
	uvec::reflect(forward, n, 0.f);
	uvec::reflect(right, n, 0.f);
	uvec::reflect(up, n, 0.f);

	auto &origin = GetOrigin();
	uvec::reflect(origin, n, d);

	rot = uquat::create(forward, -right, up); // Reflection caused our handedness to flip, so we have to inverse 'right'
}

void umath::Transform::SetOrigin(const Vector3 &origin) { translation = origin; }

void umath::Transform::SetRotation(const Quat &rot) { rotation = rot; }

void umath::Transform::SetIdentity()
{
	translation = {};
	rotation = uquat::identity();
}
void umath::Transform::TranslateGlobal(const Vector3 &v) { translation += v; }
void umath::Transform::TranslateLocal(const Vector3 &v)
{
	auto vrot = v;
	uvec::rotate(&vrot, rotation);
	translation += vrot;
}
void umath::Transform::RotateGlobal(const Quat &rot)
{
	uvec::rotate(&translation, rot);
	rotation = rot * rotation;
}
void umath::Transform::RotateLocal(const Quat &rot) { rotation *= rot; }
void umath::Transform::Interpolate(const Transform &dst, float factor)
{
	translation = uvec::lerp(translation, dst.translation, factor);
	rotation = uquat::slerp(rotation, dst.rotation, factor);
}
void umath::Transform::InterpolateToIdentity(float factor) { Interpolate({}, factor); }
umath::ScaledTransform umath::Transform::operator*(const ScaledTransform &tOther) const { return umath::ScaledTransform {*this} * tOther; }
umath::Transform umath::Transform::operator*(const Transform &tOther) const
{
	auto res = *this;
	res *= tOther;
	return res;
}

umath::Transform &umath::Transform::operator*=(const Transform &tOther)
{
	auto t = tOther.translation;
	uvec::rotate(&t, rotation);
	rotation *= tOther.rotation;
	translation += t;
	return *this;
}
Vector3 umath::Transform::operator*(const Vector3 &t) const
{
	auto result = t;
	uvec::rotate(&result, rotation);
	result += translation;
	return result;
}
Quat umath::Transform::operator*(const Quat &rot) const { return rotation * rot; }
umath::Plane umath::Transform::operator*(const Plane &plane) const
{
	auto pos = plane.GetPos();
	auto n = plane.GetNormal();
	pos = *this * pos;
	n = GetRotation() * n;
	return Plane {n, pos};
}
umath::Transform umath::Transform::operator*(float weight) const
{
	auto res = *this;
	res *= weight;
	return res;
}
umath::Transform &umath::Transform::operator*=(float weight)
{
	translation *= weight;
	// Not entirely sure about this...
	rotation = uquat::slerp(uquat::identity(), rotation, weight);
	return *this;
}

/////////////

umath::ScaledTransform::ScaledTransform(const Transform &t, const Vector3 &scale) : Transform {t}, scale {scale} {}
umath::ScaledTransform::ScaledTransform(const Transform &t) : Transform {t} {}
umath::ScaledTransform::ScaledTransform(const Vector3 &pos, const Quat &rot, const Vector3 &scale) : Transform {pos, rot}, scale {scale} {}
void umath::ScaledTransform::SetIdentity()
{
	Transform::SetIdentity();
	scale = {1.f, 1.f, 1.f};
}
bool umath::ScaledTransform::operator==(const ScaledTransform &t) const { return Transform::operator==(t) && uvec::cmp(scale, t.scale); }
bool umath::ScaledTransform::operator==(const Transform &t) const { return Transform::operator==(t) && uvec::cmp(scale, Vector3 {1.f, 1.f, 1.f}, {0.001f, 0.001f, 0.001f}); }
const Vector3 &umath::ScaledTransform::GetScale() const { return const_cast<ScaledTransform *>(this)->GetScale(); }
Vector3 &umath::ScaledTransform::GetScale() { return scale; }
void umath::ScaledTransform::SetScale(const Vector3 &scale) { this->scale = scale; }
void umath::ScaledTransform::Scale(const Vector3 &scale) { this->scale *= scale; }
void umath::ScaledTransform::Interpolate(const ScaledTransform &dst, float factor)
{
	Transform::Interpolate(dst, factor);
	scale = uvec::lerp(scale, dst.scale, factor);
}
void umath::ScaledTransform::InterpolateToIdentity(float factor) { Interpolate({}, factor); }
umath::ScaledTransform umath::ScaledTransform::GetInverse() const
{
	auto &scale = GetScale();
	ScaledTransform inverse {Transform::GetInverse()};
	inverse.SetScale(Vector3 {1.f / scale.x, 1.f / scale.y, 1.f / scale.z}); // TODO: What to do about division by zero cases?
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
	scale *= tOther.scale;
	return *this;
}
umath::ScaledTransform &umath::ScaledTransform::operator*=(const Transform &tOther)
{
	Transform::operator*=(tOther);
	return *this;
}
Vector3 umath::ScaledTransform::operator*(const Vector3 &translation) const { 
	Vector3 v = translation * GetScale();
	return Transform::operator*(v);
}
Quat umath::ScaledTransform::operator*(const Quat &rot) const { return Transform::operator*(rot); }
umath::Plane umath::ScaledTransform::operator*(const Plane &plane) const { return Transform::operator*(plane); }

Mat4 umath::ScaledTransform::ToMatrix() const { return Transform::ToMatrix() * glm::scale(glm::mat4 {1.f}, scale); }

umath::ScaledTransform umath::ScaledTransform::operator*(float weight) const
{
	auto res = *this;
	res *= weight;
	return res;
}
umath::ScaledTransform &umath::ScaledTransform::operator*=(float weight)
{
	Transform::operator*=(weight);
	scale *= weight;
	return *this;
}

/////////////

Vector3 operator*(const Vector3 &v, const umath::Transform &t) { return (umath::Transform {v, uquat::identity()} * t).GetOrigin(); }
Vector3 &operator*=(Vector3 &v, const umath::Transform &t)
{
	v = operator*(v, t);
	return v;
}
Quat operator*(const Quat &v, const umath::Transform &t) { return (umath::Transform {Vector3 {}, v} * t).GetOrigin(); }
Quat &operator*=(Quat &v, const umath::Transform &t)
{
	v = operator*(v, t);
	return v;
}
umath::Transform operator*(float weight, const umath::Transform &t) { return t * weight; }

Vector3 operator*(const Vector3 &v, const umath::ScaledTransform &t) { return operator*(v, static_cast<const umath::Transform &>(t)); }
Vector3 &operator*=(Vector3 &v, const umath::ScaledTransform &t) { return operator*=(v, static_cast<const umath::Transform &>(t)); }
Quat operator*(const Quat &v, const umath::ScaledTransform &t) { return operator*(v, static_cast<const umath::Transform &>(t)); }
Quat &operator*=(Quat &v, const umath::ScaledTransform &t) { return operator*=(v, static_cast<const umath::Transform &>(t)); }
umath::ScaledTransform operator*(float weight, const umath::ScaledTransform &t) { return t * weight; }
