/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mathutil/inverse_kinematics/ik.hpp"

using namespace uvec::ik;

IkConstraint &IkJoint::AddConstraint(std::unique_ptr<IkConstraint> constraint)
{
	m_constraints.push_back(std::move(constraint));
	return *m_constraints.back();
}
