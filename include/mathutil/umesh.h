/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __UMESH_H__
#define __UMESH_H__

#ifdef ENABLE_MESH_FUNCTIONS
#include "mathutildefinitions.h"
#include "uvec.h"

namespace umesh
{
	DLLMUTIL bool generate_convex_hull(const std::vector<Vector3> &pointCloud,std::vector<uint32_t> &convexHull);
	DLLMUTIL std::vector<uint32_t> generate_convex_hull(const std::vector<Vector3> &pointCloud);
	DLLMUTIL void calc_smallest_enclosing_bbox(const std::vector<Vector3> &pointCloud,Vector3 &center,Vector3 &extents,Quat &rot);
};
#endif

#endif // __VECTOR3_H__
