// SPDX-FileCopyrightText: (c) 2020 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

module;

module pragma.math;

import :transform;

pragma::math::Transform::Transform(const Mat4 &t) { umat::decompose(t, translation, rotation); }

pragma::math::Transform::Transform(const Vector3 &translation, const Quat &rotation) : translation {translation}, rotation {rotation} {}

pragma::math::Transform::Transform(const Vector3 &translation) : Transform {translation, uquat::identity()} {}
pragma::math::Transform::Transform(const Quat &rotation) : Transform {Vector3 {}, rotation} {}

bool pragma::math::Transform::operator==(const Transform &t) const { return uvec::cmp(translation, t.translation, {0.001f, 0.001f, 0.001f}) && uquat::cmp(rotation, t.rotation, 0.001f); }

pragma::math::Transform pragma::math::Transform::GetInverse() const
{
	Transform result {-translation, uquat::get_inverse(rotation)};
	uvec::rotate(&result.translation, result.rotation);
	return result;
}

pragma::math::Plane pragma::math::Transform::ToPlane() const
{
	auto &origin = GetOrigin();
	auto p0 = origin;
	auto p1 = origin + GetUp() * 10.f;
	auto p2 = origin + GetRight() * 10.f;
	return Plane {p0, p1, p2};
}

EulerAngles pragma::math::Transform::GetAngles() const { return EulerAngles {GetRotation()}; }
void pragma::math::Transform::SetAngles(const EulerAngles &ang) { SetRotation(uquat::create(ang)); }
Vector3 pragma::math::Transform::GetForward() const { return uquat::forward(GetRotation()); }
Vector3 pragma::math::Transform::GetRight() const { return uquat::right(GetRotation()); }
Vector3 pragma::math::Transform::GetUp() const { return uquat::up(GetRotation()); }

const Vector3 &pragma::math::Transform::GetOrigin() const { return const_cast<Transform *>(this)->GetOrigin(); }
const Quat &pragma::math::Transform::GetRotation() const { return const_cast<Transform *>(this)->GetRotation(); }
Vector3 &pragma::math::Transform::GetOrigin() { return translation; }
Quat &pragma::math::Transform::GetRotation() { return rotation; }

Mat4 pragma::math::Transform::ToMatrix() const
{
	auto m = glm::gtx::translate(Mat4 {1.f}, translation);
	m *= umat::create(rotation);
	return m;
}

void pragma::math::Transform::Reflect(const Vector3 &n, float d)
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

void pragma::math::Transform::SetOrigin(const Vector3 &origin) { translation = origin; }

void pragma::math::Transform::SetRotation(const Quat &rot) { rotation = rot; }

void pragma::math::Transform::SetIdentity()
{
	translation = {};
	rotation = uquat::identity();
}
void pragma::math::Transform::TranslateGlobal(const Vector3 &v) { translation += v; }
void pragma::math::Transform::TranslateLocal(const Vector3 &v)
{
	auto vrot = v;
	uvec::rotate(&vrot, rotation);
	translation += vrot;
}
void pragma::math::Transform::RotateGlobal(const Quat &rot)
{
	uvec::rotate(&translation, rot);
	rotation = rot * rotation;
}
void pragma::math::Transform::RotateLocal(const Quat &rot) { rotation *= rot; }
void pragma::math::Transform::Interpolate(const Transform &dst, float factor)
{
	translation = uvec::lerp(translation, dst.translation, factor);
	rotation = uquat::slerp(rotation, dst.rotation, factor);
}
void pragma::math::Transform::InterpolateToIdentity(float factor) { Interpolate({}, factor); }
pragma::math::ScaledTransform pragma::math::Transform::operator*(const ScaledTransform &tOther) const { return ScaledTransform {*this} * tOther; }
pragma::math::Transform pragma::math::Transform::operator*(const Transform &tOther) const
{
	auto res = *this;
	res *= tOther;
	return res;
}

pragma::math::Transform &pragma::math::Transform::operator*=(const Transform &tOther)
{
	auto t = tOther.translation;
	uvec::rotate(&t, rotation);
	rotation *= tOther.rotation;
	translation += t;
	return *this;
}
Vector3 pragma::math::Transform::operator*(const Vector3 &t) const
{
	auto result = t;
	uvec::rotate(&result, rotation);
	result += translation;
	return result;
}
Quat pragma::math::Transform::operator*(const Quat &rot) const { return rotation * rot; }
pragma::math::Plane pragma::math::Transform::operator*(const Plane &plane) const
{
	auto pos = plane.GetPos();
	auto n = plane.GetNormal();
	pos = *this * pos;
	n = GetRotation() * n;
	return Plane {n, pos};
}
pragma::math::Transform pragma::math::Transform::operator*(float weight) const
{
	auto res = *this;
	res *= weight;
	return res;
}
pragma::math::Transform &pragma::math::Transform::operator*=(float weight)
{
	translation *= weight;
	// Not entirely sure about this...
	rotation = uquat::slerp(uquat::identity(), rotation, weight);
	return *this;
}

/////////////

pragma::math::ScaledTransform::ScaledTransform(const Mat4 &t) { umat::decompose(t, translation, rotation, &scale); }

pragma::math::ScaledTransform::ScaledTransform(const Transform &t, const Vector3 &scale) : Transform {t}, scale {scale} {}
pragma::math::ScaledTransform::ScaledTransform(const Transform &t) : Transform {t} {}
pragma::math::ScaledTransform::ScaledTransform(const Vector3 &pos, const Quat &rot, const Vector3 &scale) : Transform {pos, rot}, scale {scale} {}
void pragma::math::ScaledTransform::SetIdentity()
{
	Transform::SetIdentity();
	scale = {1.f, 1.f, 1.f};
}
bool pragma::math::ScaledTransform::operator==(const ScaledTransform &t) const { return Transform::operator==(t) && uvec::cmp(scale, t.scale); }
bool pragma::math::ScaledTransform::operator==(const Transform &t) const { return Transform::operator==(t) && uvec::cmp(scale, Vector3 {1.f, 1.f, 1.f}, {0.001f, 0.001f, 0.001f}); }
const Vector3 &pragma::math::ScaledTransform::GetScale() const { return const_cast<ScaledTransform *>(this)->GetScale(); }
Vector3 &pragma::math::ScaledTransform::GetScale() { return scale; }
void pragma::math::ScaledTransform::SetScale(const Vector3 &scale) { this->scale = scale; }
void pragma::math::ScaledTransform::Scale(const Vector3 &scale) { this->scale *= scale; }
void pragma::math::ScaledTransform::Interpolate(const ScaledTransform &dst, float factor)
{
	Transform::Interpolate(dst, factor);
	scale = uvec::lerp(scale, dst.scale, factor);
}
void pragma::math::ScaledTransform::InterpolateToIdentity(float factor) { Interpolate({}, factor); }
pragma::math::ScaledTransform pragma::math::ScaledTransform::GetInverse() const
{
	auto &scale = GetScale();
	ScaledTransform inverse {Transform::GetInverse()};
	inverse.SetScale(Vector3 {1.f / scale.x, 1.f / scale.y, 1.f / scale.z}); // TODO: What to do about division by zero cases?
	return inverse;
}
pragma::math::ScaledTransform pragma::math::ScaledTransform::operator*(const ScaledTransform &tOther) const
{
	auto res = *this;
	res *= tOther;
	return res;
}
pragma::math::ScaledTransform pragma::math::ScaledTransform::operator*(const Transform &tOther) const
{
	auto res = *this;
	res *= tOther;
	return res;
}
pragma::math::ScaledTransform &pragma::math::ScaledTransform::operator*=(const ScaledTransform &tOther)
{
	Transform::operator*=(tOther);
	scale *= tOther.scale;
	return *this;
}
pragma::math::ScaledTransform &pragma::math::ScaledTransform::operator*=(const Transform &tOther)
{
	Transform::operator*=(tOther);
	return *this;
}
Vector3 pragma::math::ScaledTransform::operator*(const Vector3 &translation) const
{
	Vector3 v = translation * GetScale();
	return Transform::operator*(v);
}
Quat pragma::math::ScaledTransform::operator*(const Quat &rot) const { return Transform::operator*(rot); }
pragma::math::Plane pragma::math::ScaledTransform::operator*(const Plane &plane) const { return Transform::operator*(plane); }

Mat4 pragma::math::ScaledTransform::ToMatrix() const { return Transform::ToMatrix() * glm::gtx::scale(glm::mat4 {1.f}, scale); }

pragma::math::ScaledTransform pragma::math::ScaledTransform::operator*(float weight) const
{
	auto res = *this;
	res *= weight;
	return res;
}
pragma::math::ScaledTransform &pragma::math::ScaledTransform::operator*=(float weight)
{
	Transform::operator*=(weight);
	scale *= weight;
	return *this;
}

/////////////

Vector3 operator*(const Vector3 &v, const pragma::math::Transform &t) { return (pragma::math::Transform {v, uquat::identity()} * t).GetOrigin(); }
Vector3 &operator*=(Vector3 &v, const pragma::math::Transform &t)
{
	v = operator*(v, t);
	return v;
}
Quat operator*(const Quat &v, const pragma::math::Transform &t) { return (pragma::math::Transform {Vector3 {}, v} * t).GetOrigin(); }
Quat &operator*=(Quat &v, const pragma::math::Transform &t)
{
	v = operator*(v, t);
	return v;
}
pragma::math::Transform operator*(float weight, const pragma::math::Transform &t) { return t * weight; }

Vector3 operator*(const Vector3 &v, const pragma::math::ScaledTransform &t) { return operator*(v, static_cast<const pragma::math::Transform &>(t)); }
Vector3 &operator*=(Vector3 &v, const pragma::math::ScaledTransform &t) { return operator*=(v, static_cast<const pragma::math::Transform &>(t)); }
Quat operator*(const Quat &v, const pragma::math::ScaledTransform &t) { return operator*(v, static_cast<const pragma::math::Transform &>(t)); }
Quat &operator*=(Quat &v, const pragma::math::ScaledTransform &t) { return operator*=(v, static_cast<const pragma::math::Transform &>(t)); }
pragma::math::ScaledTransform operator*(float weight, const pragma::math::ScaledTransform &t) { return t * weight; }

void pragma::math::transform(const Vector3 &parentPos, const Quat &parentRot, const Vector3 *optParentScale, Vector3 &pos, Quat &rot, Vector3 *optScale)
{
	uvec::rotate(&pos, parentRot);
	rot = parentRot * rot;
	pos += parentPos;
	if(optParentScale && optScale)
		*optScale *= *optParentScale;
}
