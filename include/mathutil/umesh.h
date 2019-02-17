#ifndef __UMESH_H__
#define __UMESH_H__

#include "mathutildefinitions.h"
#include "uvec.h"

namespace umesh
{
	DLLMUTIL bool generate_convex_hull(const std::vector<Vector3> &pointCloud,std::vector<uint32_t> &convexHull);
	DLLMUTIL std::vector<uint32_t> generate_convex_hull(const std::vector<Vector3> &pointCloud);
	DLLMUTIL void calc_smallest_enclosing_bbox(const std::vector<Vector3> &pointCloud,Vector3 &center,Vector3 &extents,Quat &rot);
};

#endif // __VECTOR3_H__
