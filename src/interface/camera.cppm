// SPDX-FileCopyrightText: (c) 2020 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

module;

#include "definitions.hpp"

export module pragma.math:camera;

export import :types;

export namespace umath::camera {
	DLLMUTIL float calc_fov_from_lens(umath::Millimeter sensorSize, umath::Millimeter focalLength, float aspectRatio);
	DLLMUTIL float calc_aperture_size_from_fstop(float fstop, umath::Millimeter focalLength, bool orthographicCamera = false);
	DLLMUTIL float calc_focal_length_from_fov(umath::Degree hfov, umath::Millimeter sensorSize);
	DLLMUTIL float calc_fov_from_focal_length(umath::Millimeter focalLength, umath::Millimeter sensorSize);
};
