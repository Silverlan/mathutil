// SPDX-FileCopyrightText: (c) 2019 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

export module pragma.math:equation_solver;

export import std.compat;

export namespace pragma::math {
	DLLMUTIL int32_t solve_quadric(const std::array<double, 3> &c, std::array<double, 2> &s);
	DLLMUTIL int32_t solve_cubic(const std::array<double, 4> &c, std::array<double, 3> &s);
	DLLMUTIL int32_t solve_quartic(const std::array<double, 5> &c, std::array<double, 4> &s);
};
