/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __UMATH_INVERSE_KINEMATICS_CONSTRAINTS_HPP__
#define __UMATH_INVERSE_KINEMATICS_CONSTRAINTS_HPP__

#include "mathutil/mathutildefinitions.h"
#include "mathutil/uvec.h"
#include <optional>

namespace uvec::ik
{
	class IkJoint;
	class DLLMUTIL IkConstraint
	{
	public:
		IkConstraint(IkJoint &joint)
			: m_joint{joint}
		{}
		virtual void Apply(int i)=0;
		IkJoint &GetJoint() const {return m_joint;}
	public:
		IkJoint &m_joint;
	};

	class DLLMUTIL IkHingeConstraint
		: public IkConstraint
	{
	public:
		IkHingeConstraint(IkJoint &joint,const Vector3 &axis)
			: IkConstraint{joint},m_axis{axis}
		{}
		virtual void Apply(int i) override;
		void SetLimits(const Vector2 &limits);
		void ClearLimits();
		std::optional<Vector2> GetLimits() const;
	private:
		Vector3 m_axis {0.f,0.f,1.f};
		std::optional<Vector2> m_limits {};
	};

	class DLLMUTIL IkBallSocketConstraint
		: public IkConstraint
	{
	public:
		IkBallSocketConstraint(IkJoint &joint,float limit)
			: IkConstraint{joint},m_limit{limit}
		{}
		virtual void Apply(int i) override;

		void SetLimit(float limit) {m_limit = limit;}
		bool GetLimit(float &outLimit) const {return m_limit;}
	private:
		float m_limit = 0.f;
	};
};

#endif
