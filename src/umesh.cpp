/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifdef ENABLE_MESH_FUNCTIONS
#include "mathutil/umesh.h"
#include <Mathematics/MinimumVolumeBox3.h>

bool umesh::generate_convex_hull(const std::vector<Vector3> &pointCloud,std::vector<uint32_t> &convexHull)
{
	if(pointCloud.size() < 4)
		return false;
	gte::ConvexHull3<float> hull{};
	hull(static_cast<size_t>(pointCloud.size()),reinterpret_cast<const gte::Vector3<float>*>(pointCloud.data()),3);
	if(hull.GetDimension() < 3)
		return false;
	auto &hullMesh = hull.GetHullMesh();
	auto &triangles = hullMesh.GetTriangles();
	convexHull.reserve(convexHull.size() +triangles.size() *3);
	for(auto &it : triangles)
	{
		convexHull.push_back(it.second->V[0]);
		convexHull.push_back(it.second->V[1]);
		convexHull.push_back(it.second->V[2]);
	}
	return true;
}

std::vector<uint32_t> umesh::generate_convex_hull(const std::vector<Vector3> &pointCloud)
{
	std::vector<uint32_t> indices;
	generate_convex_hull(pointCloud,indices);
	return indices;
}

void umesh::calc_smallest_enclosing_bbox(const std::vector<Vector3> &pointCloud,Vector3 &center,Vector3 &extents,Quat &rot)
{
	gte::MinimumVolumeBox3<decltype(Vector3::x),false> vol{};
	auto &gtVerts = reinterpret_cast<const std::vector<gte::Vector3<decltype(Vector3::x)>>&>(pointCloud);
	std::vector<uint32_t> indices;
	generate_convex_hull(pointCloud,indices);
	auto &gtIndices = reinterpret_cast<std::vector<int32_t>&>(indices);
	gte::OrientedBox3<float> minBox;
	float volume;
	vol(static_cast<int32_t>(gtVerts.size()),gtVerts.data(),static_cast<int32_t>(gtIndices.size()),gtIndices.data(),2 /* maxSample */,minBox,volume);
	std::array<Vector3,8> boxVerts;
	minBox.GetVertices(reinterpret_cast<std::array<gte::Vector3<decltype(Vector3::x)>,8>&>(boxVerts));

	center = (boxVerts[0] +boxVerts[7]) *0.5f;
	auto axis1 = boxVerts[2] -boxVerts[0];
	auto axis2 = boxVerts[1] -boxVerts[0];
	auto axis3 = boxVerts[4] -boxVerts[0];
	extents = axis1 *0.5f +axis2 *0.5f +axis3 *0.5f;
	uvec::normalize(&axis1);
	uvec::normalize(&axis2);
	uvec::normalize(&axis3);
	rot = uquat::create(axis1,axis2,axis3);
	extents = extents *rot;
}
#endif
