// SPDX-FileCopyrightText: (c) 2021 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

module;

module pragma.math;

import :ik.core;
import :ik.constraints;

using namespace uvec::ik;

void IkHingeConstraint::SetLimits(const Vector2 &limits) { m_limits = limits; }
void IkHingeConstraint::ClearLimits() { m_limits = {}; }
std::optional<Vector2> IkHingeConstraint::GetLimits() const { return m_limits; }

void IkHingeConstraint::Apply(int i)
{
	if(i == 0)
		return;
	auto joint = m_joint.GetIkSolver().GetGlobalTransform(i);
	auto parent = m_joint.GetIkSolver().GetGlobalTransform(i - 1);

	auto currentHinge = joint.GetRotation() * m_axis;
	auto desiredHinge = parent.GetRotation() * m_axis;

	auto rot = uvec::get_rotation(currentHinge, desiredHinge);

	auto localTransform = m_joint.GetIkSolver().GetLocalTransform(i);
	auto angle = uvec::get_angle(uquat::forward(localTransform.GetRotation()), PRM_FORWARD);

	auto localRot = rot * localTransform.GetRotation();
	if(m_limits.has_value()) {
		auto tmpRot = uquat::create_look_rotation(uvec::get_normal(uvec::cross(m_axis, PRM_RIGHT)), m_axis);
		localRot = uquat::get_inverse(tmpRot) * localRot;
		localRot = uquat::clamp_rotation(localRot, EulerAngles {0.f, m_limits->x, 0.f}, EulerAngles {0.f, m_limits->y, 0.f});
		localRot = tmpRot * localRot;
	}

	localTransform.SetRotation(localRot);
	m_joint.GetIkSolver().SetLocalTransform(i, localTransform);
}

/////

#define VEC3_EPSILON 0.000001f
#define QUAT_DEG2RAD 0.0174533f
static float angle(const Vector3 &l, const Vector3 &r)
{
	float sqMagL = l.x * l.x + l.y * l.y + l.z * l.z;
	float sqMagR = r.x * r.x + r.y * r.y + r.z * r.z;

	if(sqMagL < VEC3_EPSILON || sqMagR < VEC3_EPSILON) {
		return 0.0f;
	}

	float dot = l.x * r.x + l.y * r.y + l.z * r.z;
	float len = sqrtf(sqMagL) * sqrtf(sqMagR);
	return acosf(dot / len);
}
void IkBallSocketConstraint::Apply(int i)
{
	pragma::math::ScaledTransform mOffset {};                                                                               // TODO: What's this?
	auto parentRot = i == 0 ? mOffset.GetRotation() : m_joint.GetIkSolver().GetGlobalTransform(i - 1).GetRotation(); // GetWorldTransform?
	auto thisRot = m_joint.GetIkSolver().GetGlobalTransform(i).GetRotation();                                        // GetWorldTransform?
	auto parentDir = parentRot * Vector3(0, 0, 1);
	auto thisDir = thisRot * Vector3(0, 0, 1);
	float angle = ::angle(parentDir, thisDir);
	if(angle > m_limit * QUAT_DEG2RAD) {
		auto correction = uvec::cross(parentDir, thisDir);
		uvec::normalize(&correction);
		auto worldSpaceRotation = glm::gtc::angleAxis(m_limit * QUAT_DEG2RAD, correction) * parentRot;
		m_joint.GetIkSolver().GetJointPose(i).SetRotation(inverse(parentRot) * worldSpaceRotation);
	}
}
