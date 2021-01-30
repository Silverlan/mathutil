/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __UMATH_INVERSE_KINEMATICS_HPP__
#define __UMATH_INVERSE_KINEMATICS_HPP__

#include "mathutil/mathutildefinitions.h"
#include "mathutil/uvec.h"
#include "mathutil/transform.hpp"
#include "mathutil/inverse_kinematics/constraints.hpp"
#include <memory>

namespace uvec::ik
{
	class IkConstraint;
	class IkSolver;

	class DLLMUTIL IkJoint
	{
	public:
		IkJoint()=default;
		IkJoint(IkSolver &solver)
			: m_ikSolver{&solver}
		{}
		IkJoint(const IkJoint&)=delete;
		IkJoint &operator=(const IkJoint&)=delete;
		IkJoint(IkJoint &&other)
			: m_ikSolver{other.m_ikSolver},
			m_jointIndex{other.m_jointIndex},
			m_constraints{std::move(other.m_constraints)},
			m_pose{other.m_pose}
		{}
		IkJoint &operator=(IkJoint &&other)
		{
			m_ikSolver = other.m_ikSolver;
			m_jointIndex = other.m_jointIndex;
			m_constraints = std::move(other.m_constraints);
			m_pose = other.m_pose;
			return *this;
		}
		IkSolver &GetIkSolver() const {return *m_ikSolver;}
		const std::vector<std::unique_ptr<IkConstraint>> &GetConstraints() const {return m_constraints;}

		void SetJointIndex(uint32_t jointIndex) {m_jointIndex = jointIndex;}
		uint32_t GetJointIndex() const {return m_jointIndex;}

		umath::ScaledTransform &GetPose() {return m_pose;}
		const umath::ScaledTransform &GetPose() const {return const_cast<IkJoint*>(this)->GetPose();}

		bool HasConstraints() const {return !m_constraints.empty();}
		IkConstraint &AddConstraint(std::unique_ptr<IkConstraint> constraint);
		template<class TConstraint,typename... TARGS>
			TConstraint &AddConstraint(TARGS ...args)
		{
			return static_cast<TConstraint&>(AddConstraint(std::make_unique<TConstraint>(*this,std::forward<TARGS>(args)...)));
		}
	protected:
		IkSolver *m_ikSolver = nullptr;
		uint32_t m_jointIndex = 0;
		std::vector<std::unique_ptr<IkConstraint>> m_constraints;
		umath::ScaledTransform m_pose {};
	};

	class DLLMUTIL IkSolver
	{
	public:
		IkSolver()=default;
		IkSolver(const IkSolver&)=delete;
		IkSolver &operator=(const IkSolver&)=delete;
		virtual ~IkSolver()=default;
		umath::ScaledTransform GetGlobalTransform(unsigned int index) const;
		umath::ScaledTransform &GetJointPose(uint32_t idx) {return mIKChain[idx].GetPose();}
		void ApplyConstraints();
		void ApplyConstraints(uint32_t iConstraint);
		unsigned int Size() {return mIKChain.size();}
		virtual void Resize(unsigned int newSize);
		virtual bool Solve(const umath::ScaledTransform& target)=0;

		umath::ScaledTransform GetLocalTransform(unsigned int index);
		void SetLocalTransform(unsigned int index, const umath::ScaledTransform& t);

		IkJoint &GetJoint(uint32_t i) {return mIKChain[i];}
		const IkJoint &GetJoint(uint32_t i) const {return const_cast<IkSolver*>(this)->GetJoint(i);}
	protected:
		std::vector<IkJoint> mIKChain;
	};

	class DLLMUTIL CCDSolver
		: public IkSolver
	{
	public:
		CCDSolver();

		unsigned int GetNumSteps();
		void SetNumSteps(unsigned int numSteps);

		float GetThreshold();
		void SetThreshold(float value);
		virtual bool Solve(const umath::ScaledTransform& target) override;
	protected:
		unsigned int mNumSteps;
		float mThreshold;
	};

	class DLLMUTIL FABRIKSolver
		: public IkSolver
	{
	public:
		FABRIKSolver();

		virtual void Resize(unsigned int newSize) override;

		unsigned int GetNumSteps();
		void SetNumSteps(unsigned int numSteps);

		float GetThreshold();
		void SetThreshold(float value);

		virtual bool Solve(const umath::ScaledTransform& target) override;
	protected:
		unsigned int mNumSteps;
		float mThreshold;
		std::vector<Vector3> mWorldChain;
		std::vector<float> mLengths;
	protected:
		void IKChainToWorld();
		void IterateForward(const Vector3& goal);
		void IterateBackward(const Vector3& base);
		void WorldToIKChain();
	};
};

#endif
