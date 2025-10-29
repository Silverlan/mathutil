// SPDX-FileCopyrightText: (c) 2021 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

module;


module pragma.math;

import :ik.core;
import :ik.constraints;

using namespace uvec::ik;

IkConstraint &IkJoint::AddConstraint(std::unique_ptr<IkConstraint> constraint)
{
	m_constraints.push_back(std::move(constraint));
	return *m_constraints.back();
}
