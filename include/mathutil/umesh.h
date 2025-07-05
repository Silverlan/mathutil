// SPDX-FileCopyrightText: (c) 2019 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

#ifndef __UMESH_H__
#define __UMESH_H__

#ifdef ENABLE_MESH_FUNCTIONS
#include "mathutildefinitions.h"
#include "uvec.h"

namespace umesh {
	DLLMUTIL bool generate_convex_hull(const std::vector<Vector3> &pointCloud, std::vector<uint32_t> &convexHull);
	DLLMUTIL std::vector<uint32_t> generate_convex_hull(const std::vector<Vector3> &pointCloud);
	DLLMUTIL void calc_smallest_enclosing_bbox(const std::vector<Vector3> &pointCloud, Vector3 &center, Vector3 &extents, Quat &rot);
};
#endif

#endif // __VECTOR3_H__
