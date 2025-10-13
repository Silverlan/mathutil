// SPDX-FileCopyrightText: (c) 2019 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

module;

#include "mathutil/mathutildefinitions.h"
#include "mathutil/glmutil.h"
#include <string>

export module pragma.math:matrix;

export import :types;

export namespace umat {
	const Mat4 BIAS(0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.5, 0.5, 0.5, 1.0);
	DLLMUTIL Mat4 create(const Quat &q);
	DLLMUTIL Mat4 create_from_axis_angle(const Vector3 &v, Float ang);
	DLLMUTIL Mat4 create_from_axes(const Vector3 &forward, const Vector3 &right, const Vector3 &up);
	DLLMUTIL Mat4 create_reflection(const Vector3 &n, float d);
	DLLMUTIL void to_axes(const Mat4 &m, Vector3 &outForward, Vector3 &outRight, Vector3 &outUp);
	// Safer than glm::lookat (No division by 0 possible)
	DLLMUTIL Mat4 look_at(const Vector3 &eye, const Vector3 &center, const Vector3 &up);
	DLLMUTIL Mat3 calc_covariance_matrix(const std::vector<Vector3> &points);
	DLLMUTIL Mat3 calc_covariance_matrix(const std::vector<Vector3> &points, const Vector3 &avg);
	DLLMUTIL Mat4 identity();

	DLLMUTIL void decompose(const Mat4 &t, Vector3 &outTranslation, Mat3 &outRotation, Vector3 *outScale = nullptr);
	DLLMUTIL void decompose(const Mat4 &t, Vector3 &outTranslation, Quat &outRotation, Vector3 *outScale = nullptr);
	DLLMUTIL Quat get_rotation(const Mat4 &m);

	DLLMUTIL float calc_projection_depth_bias_offset(float p, float nearZ, float farZ, float d, float delta);
	DLLMUTIL void apply_projection_depth_bias_offset(Mat4 &inOutP, float nearZ, float farZ, float d, float delta);

	DLLMUTIL Vector2 to_screen_uv(const Vector3 &point, const Mat4 &vp);
	template<typename T>
	    requires(umath::is_matrix_type<T>)
	std::string to_string(const T &m, char sep = ',')
	{
		std::string s;
		constexpr auto len = T::length();
		for(auto i = decltype(len) {0u}; i < len; ++i) {
			constexpr auto lenc = T::col_type::length();
			for(auto j = decltype(lenc) {0u}; j < lenc; ++j) {
				if(i > 0 || j > 0)
					s += sep;
				s += std::to_string(m[i][j]);
			}
		}
		return s;
	}
};
