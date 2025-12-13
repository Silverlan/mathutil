// SPDX-FileCopyrightText: (c) 2020 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

module;

#include "definitions.hpp"

export module pragma.math:camera;

export import :types;

export namespace pragma::math::camera {
	DLLMUTIL float calc_fov_from_lens(Millimeter sensorSize, Millimeter focalLength, float aspectRatio);
	DLLMUTIL float calc_aperture_size_from_fstop(float fstop, Millimeter focalLength, bool orthographicCamera = false);
	DLLMUTIL float calc_focal_length_from_fov(Degree hfov, Millimeter sensorSize);
	DLLMUTIL float calc_fov_from_focal_length(Millimeter focalLength, Millimeter sensorSize);
};
