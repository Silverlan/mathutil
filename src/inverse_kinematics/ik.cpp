/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mathutil/inverse_kinematics/ik.hpp"
#include "mathutil/inverse_kinematics/constraints.hpp"

using namespace uvec::ik;
#pragma optimize("",off)
static Quat fromTo(const Vector3& from, const Vector3& to)
{
	auto f = uvec::get_normal(from);
	auto t = uvec::get_normal(to);

	if (f == t) {
		return uquat::identity();
	}
	else if (f == t * -1.0f) {
		auto ortho = Vector3(1, 0, 0);
		if (fabsf(f.y) < fabsf(f.x)) {
			ortho = Vector3(0, 1, 0);
		}
		if (fabsf(f.z) < fabs(f.y) && fabs(f.z) < fabsf(f.x)) {
			ortho = Vector3(0, 0, 1);
		}

		auto axis = uvec::get_normal(uvec::cross(f, ortho));
		return Quat(0,axis.x, axis.y, axis.z);
	}

	auto half = uvec::get_normal(f + t);
	auto axis = uvec::cross(f, half);

	return Quat(
		dot(f, half),
		axis.x,
		axis.y,
		axis.z
	);
}

CCDSolver::CCDSolver()
{
	mNumSteps = 15;
	mThreshold = 0.00001f;
}

unsigned int CCDSolver::GetNumSteps() {
	return mNumSteps;
}

void CCDSolver::SetNumSteps(unsigned int numSteps) {
	mNumSteps = numSteps;
}

float CCDSolver::GetThreshold() {
	return mThreshold;
}

void CCDSolver::SetThreshold(float value) {
	mThreshold = value;
}

bool CCDSolver::Solve(const umath::ScaledTransform& target) {
	unsigned int size = Size();
	if (size == 0) { return false; }
	unsigned int last = size - 1;
	float thresholdSq = mThreshold * mThreshold;
	auto goal = target.GetOrigin();
	for (unsigned int i = 0; i < mNumSteps; ++i) {
		auto effector = GetGlobalTransform(last).GetOrigin();
		if (uvec::length_sqr(goal -effector) < thresholdSq) {
			return true;
		}
		for (int j = (int)size - 2; j >= 0; --j) {
			effector = GetGlobalTransform(last).GetOrigin();

			auto world = GetGlobalTransform(j);
			auto position = world.GetOrigin();
			auto rotation = world.GetRotation();

			auto toEffector = effector - position;
			auto toGoal = goal - position;
			Quat effectorToGoal = uquat::identity();
			if (uvec::length_sqr(toGoal) > 0.00001f) {
				effectorToGoal = fromTo(toEffector, toGoal);
			}

			auto worldRotated = effectorToGoal *rotation;
			auto localRotate = inverse(rotation) *worldRotated;
			mIKChain[j].GetPose().SetRotation(mIKChain[j].GetPose().GetRotation() *localRotate);
			ApplyConstraints();
			effector = GetGlobalTransform(last).GetOrigin();
			if (uvec::length_sqr(goal - effector) < thresholdSq) {
				return true;
			}
		}
	}

	return false;
}

/////

FABRIKSolver::FABRIKSolver() {
	mNumSteps = 15;
	mThreshold = 0.00001f;
}

unsigned int FABRIKSolver::GetNumSteps() {
	return mNumSteps;
}

void FABRIKSolver::SetNumSteps(unsigned int numSteps) {
	mNumSteps = numSteps;
}

float FABRIKSolver::GetThreshold() {
	return mThreshold;
}

void FABRIKSolver::SetThreshold(float value) {
	mThreshold = value;
}

void FABRIKSolver::Resize(unsigned int newSize) {
	IkSolver::Resize(newSize);
	mWorldChain.resize(newSize);
	mLengths.resize(newSize);
}

void FABRIKSolver::IKChainToWorld() {
	unsigned int size = Size();
	for (unsigned int i = 0; i < size; ++i) {
		auto world = GetGlobalTransform(i);
		mWorldChain[i] = world.GetOrigin();

		if (i >= 1) {
			auto prev = mWorldChain[i - 1];
			mLengths[i] = uvec::length(world.GetOrigin() - prev);
		}
	}
	if (size > 0) {
		mLengths[0] = 0.0f;
	}
}

void FABRIKSolver::WorldToIKChain() {
	unsigned int size = Size();
	if (size == 0) { return; }

	for (unsigned int i = 0; i < size - 1; ++i) {
		auto world = GetGlobalTransform(i);
		auto next = GetGlobalTransform(i + 1);
		auto position = world.GetOrigin();
		auto rotation = world.GetRotation();

		auto toNext = next.GetOrigin() - position;
		toNext = inverse(rotation) * toNext;

		auto toDesired = mWorldChain[i + 1] - position;
		toDesired = inverse(rotation) * toDesired;

		auto delta = fromTo(toNext, toDesired);
		mIKChain[i].GetPose().SetRotation(mIKChain[i].GetPose().GetRotation() *delta);
	}
}

void FABRIKSolver::IterateBackward(const Vector3& goal) {
	int size = (int)Size();
	if (size > 0) {
		mWorldChain[size - 1] = goal;
	}

	for (int i = size - 2; i >= 0; --i) {
		auto direction = uvec::get_normal(mWorldChain[i] - mWorldChain[i + 1]);
		auto offset = direction * mLengths[i + 1];
		mWorldChain[i] = mWorldChain[i + 1] + offset;
	}
}

void FABRIKSolver::IterateForward(const Vector3& base) {
	unsigned int size = Size();
	if (size > 0) {
		mWorldChain[0] = base;
	}

	for (int i = 1; i < size; ++i) {
		auto direction = uvec::get_normal(mWorldChain[i] - mWorldChain[i - 1]);
		auto offset = direction * mLengths[i];
		mWorldChain[i] = mWorldChain[i - 1] + offset;
	}
}

bool FABRIKSolver::Solve(const umath::ScaledTransform& target) {
	unsigned int size = Size();
	if (size == 0) { return false; }
	unsigned int last = size - 1;
	float thresholdSq = mThreshold * mThreshold;

	IKChainToWorld();
	auto goal = target.GetOrigin();
	auto base = mWorldChain[0];

	for (unsigned int i = 0; i < mNumSteps; ++i) {
		auto effector = mWorldChain[last];
		if (uvec::length_sqr(goal - effector) < thresholdSq) {
			WorldToIKChain();
			return true;
		}

		IterateBackward(goal);
		IterateForward(base);

		WorldToIKChain();
		ApplyConstraints();
		IKChainToWorld();
	}

	WorldToIKChain();
	auto effector = GetGlobalTransform(last).GetOrigin();
	if (uvec::length_sqr(goal - effector) < thresholdSq) {
		return true;
	}

	return false;
}

/////

umath::ScaledTransform IkSolver::GetLocalTransform(unsigned int index)
{
	return mIKChain[index].GetPose();
}
void IkSolver::SetLocalTransform(unsigned int index, const umath::ScaledTransform& t)
{
	mIKChain[index].GetPose() = t;
}

void IkSolver::Resize(unsigned int newSize)
{
	mIKChain.resize(newSize);
	for(auto &joint : mIKChain)
		joint = IkJoint{*this};
}

void IkSolver::ApplyConstraints()
{
	for(auto i=decltype(mIKChain.size()){0u};i<mIKChain.size();++i)
	{
		for(auto &c : mIKChain[i].GetConstraints())
			c->Apply(i);
	}
}

void IkSolver::ApplyConstraints(uint32_t iConstraint)
{
	for(auto &c : mIKChain[iConstraint].GetConstraints())
		c->Apply(iConstraint);
}

umath::ScaledTransform IkSolver::GetGlobalTransform(unsigned int index) const {
	unsigned int size = (unsigned int)mIKChain.size();
	auto world = mIKChain[index].GetPose();
	for (int i = (int)index - 1; i >= 0; --i) {
		world = mIKChain[i].GetPose() *world;
	}
	return world;
}
#pragma optimize("",on)
