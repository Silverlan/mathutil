/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __UMATH_GEOMETRY_HPP__
#define __UMATH_GEOMETRY_HPP__

#include "mathutildefinitions.h"
#include "uvec.h"
#include "boundingvolume.h"
#include "plane.hpp"
#include <functional>
#include <optional>
#include <string>

namespace umath::intersection {
	enum class Result : uint32_t {
		NoIntersection = 0,
		Intersect,
		OutOfRange // There is an intersection, but not within the specified range (t will be < 0 or > 1)
	};
	enum class Intersect : uint8_t { Outside = 0, Inside, Overlap };
	DLLMUTIL bool vector_in_bounds(const Vector3 &vec, const Vector3 &min, const Vector3 &max, float EPSILON = 0.f);

	// Return true if the first AABB is contained within the second AABB
	DLLMUTIL bool aabb_in_aabb(const Vector3 &minA, const Vector3 &maxA, const Vector3 &minB, const Vector3 &maxB);
	DLLMUTIL bool point_in_aabb(const Vector3 &p, const Vector3 &min, const Vector3 &max);

	DLLMUTIL Intersect aabb_aabb(const Vector3 &minA, const Vector3 &maxA, const Vector3 &minB, const Vector3 &maxB);
	DLLMUTIL bool aabb_aabb(const bounding_volume::AABB &a, const bounding_volume::AABB &b);
	DLLMUTIL bool aabb_triangle(const Vector3 &min, const Vector3 &max, const Vector3 &a, const Vector3 &b, const Vector3 &c);
	DLLMUTIL bool aabb_plane(const Vector3 &min, const Vector3 &max, const Vector3 &n, double d);
	DLLMUTIL bool obb_plane(const Vector3 &min, const Vector3 &max, const Vector3 &origin, const Quat &rot, const Vector3 &n, double d);
	DLLMUTIL bool sphere_plane(const Vector3 &sphereOrigin, float sphereRadius, const Vector3 &n, double d);
	DLLMUTIL Result line_aabb(const Vector3 &o, const Vector3 &d, const Vector3 &min, const Vector3 &max, float *tMinRes, float *tMaxRes = NULL);
	DLLMUTIL Result line_plane(const Vector3 &o, const Vector3 &d, const Vector3 &nPlane, float distPlane, float *t = NULL);
	DLLMUTIL bool line_obb(const Vector3 &rayStart, const Vector3 &rayDir, const Vector3 &min, const Vector3 &max, float *dist = nullptr, const Vector3 &origin = {}, const Quat &rot = uquat::identity());

	DLLMUTIL bool line_sphere(const Vector3 &lineOrigin, const Vector3 &lineDir, const Vector3 &sphereOrigin, float sphereRadius, float &outT, Vector3 &outP);
	DLLMUTIL bool sphere_sphere(const Vector3 &originA, float rA, const Vector3 &originB, float rB);
	DLLMUTIL bool aabb_sphere(const Vector3 &min, const Vector3 &max, const Vector3 &origin, float r);
	DLLMUTIL bool point_in_plane_mesh(const Vector3 &vec, const std::vector<Plane> &planes);
	DLLMUTIL Intersect sphere_in_plane_mesh(const Vector3 &vec, float radius, const std::vector<Plane> &planes, bool skipInsideTest = false);
	DLLMUTIL Intersect aabb_in_plane_mesh(const Vector3 &min, const Vector3 &max, const std::vector<Plane> &planes);
	DLLMUTIL Intersect triangle_in_plane_mesh(const Vector3 &a, const Vector3 &b, const Vector3 &c, const std::vector<Plane> &planes);
	DLLMUTIL bool sphere_cone(const Vector3 &sphereOrigin, float radius, const Vector3 &coneOrigin, const Vector3 &coneDir, float coneAngle);
	DLLMUTIL bool sphere_cone(const Vector3 &sphereOrigin, float radius, const Vector3 &coneOrigin, const Vector3 &coneDir, float coneAngle, float coneSize);
	DLLMUTIL bool line_triangle(const Vector3 &lineOrigin, const Vector3 &lineDir, const Vector3 &v0, const Vector3 &v1, const Vector3 &v2, double &t, double &u, double &v, bool bCull = false);
	DLLMUTIL std::optional<Vector2> line_line(const Vector2 &start0, const Vector2 &end0, const Vector2 &start1, const Vector2 &end1);
};

namespace umath::geometry {
	enum class WindingOrder : uint8_t { Clockwise = 0u, CounterClockwise };
	DLLMUTIL void closest_point_on_aabb_to_point(const Vector3 &min, const Vector3 &max, const Vector3 &point, Vector3 *res);
	DLLMUTIL void closest_point_on_plane_to_point(const Vector3 &n, float d, const Vector3 &p, Vector3 *res);
	DLLMUTIL void closest_point_on_triangle_to_point(const Vector3 &a, const Vector3 &b, const Vector3 &c, const Vector3 &p, Vector3 *res);
	DLLMUTIL float closest_points_between_lines(const Vector3 &pA, const Vector3 &qA, const Vector3 &pB, const Vector3 &qB, float *s, float *t, Vector3 *cA, Vector3 *cB);
	DLLMUTIL Vector3 closest_point_on_line_to_point(const Vector3 &start, const Vector3 &end, const Vector3 &p, bool bClampResultToSegment = true);
	DLLMUTIL Vector3 closest_point_on_sphere_to_line(const Vector3 &origin, float radius, const Vector3 &start, const Vector3 &end, bool bClampResultToSegment = true);

	DLLMUTIL void generate_truncated_cone_mesh(const Vector3 &origin, float startRadius, const Vector3 &dir, float dist, float endRadius, std::vector<Vector3> &verts, std::vector<uint16_t> *triangles = nullptr, std::vector<Vector3> *normals = nullptr, uint32_t segmentCount = 12,
	  bool bAddCaps = true);
	DLLMUTIL void generate_truncated_elliptic_cone_mesh(const Vector3 &origin, float startRadiusX, float startRadiusY, const Vector3 &dir, float dist, float endRadiusX, float endRadiusY, std::vector<Vector3> &verts, std::vector<uint16_t> *triangles = nullptr,
	  std::vector<Vector3> *normals = nullptr, uint32_t segmentCount = 12, bool bAddCaps = true);

	DLLMUTIL double calc_volume_of_triangle(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2);
	DLLMUTIL double calc_volume_of_polyhedron(const std::function<bool(const Vector3 **, const Vector3 **, const Vector3 **)> &fGetNextTriangle, Vector3 *centerOfMass = nullptr);
	DLLMUTIL double calc_volume_of_polyhedron(const std::vector<Vector3> &verts, const std::vector<uint16_t> &triangles, Vector3 *centerOfMass = nullptr);
	DLLMUTIL Vector3 calc_center_of_mass(const std::vector<Vector3> &verts, const std::vector<uint16_t> &triangles, double *volume = nullptr);

	DLLMUTIL bool calc_barycentric_coordinates(const Vector3 &p0, const Vector3 &p1, const Vector3 &p2, const Vector3 &hitPoint, float &b1, float &b2);
	DLLMUTIL bool calc_barycentric_coordinates(const Vector3 &p0, const Vector2 &uv0, const Vector3 &p1, const Vector2 &uv1, const Vector3 &p2, const Vector2 &uv2, const Vector3 &hitPoint, float &u, float &v);
	DLLMUTIL bool calc_barycentric_coordinates(const Vector2 uv0, const Vector2 &uv1, const Vector2 &uv2, const Vector2 &uv, float &a1, float &a2, float &a3);
	DLLMUTIL Quat calc_rotation_between_planes(const Vector3 &n0, const Vector3 &n1);

	enum class LineSide : uint8_t { Left = 0u, Right, OnLine };
	DLLMUTIL LineSide get_side_of_point_to_line(const Vector2 &lineStart, const Vector2 &lineEnd, const Vector2 &point);

	enum class PlaneSide : uint8_t { Front = 0u, Back, OnPlane };
	DLLMUTIL PlaneSide get_side_of_point_to_plane(const Vector3 &n, double d, const Vector3 &p);
	DLLMUTIL void local_plane_to_world_space(Vector3 &inOutN, double &inOutD, const Vector3 &pos, const Quat &rot);

	DLLMUTIL std::optional<std::vector<uint32_t>> get_outline_vertices(const std::vector<Vector2> &polygons);
	DLLMUTIL WindingOrder get_triangle_winding_order(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2, const Vector3 &n);
	DLLMUTIL WindingOrder get_triangle_winding_order(const Vector2 &v0, const Vector2 &v1, const Vector2 &v2);

	DLLMUTIL float calc_triangle_area(const Vector3 &p0, const Vector3 &p1, const Vector3 &p2);
	DLLMUTIL float calc_triangle_area(const Vector2 &p0, const Vector2 &p1, const Vector2 &p2, bool keepSign = false);
};

namespace umath::sweep {
	DLLMUTIL bool aabb_with_aabb(const Vector3 &aa, const Vector3 &ab, const Vector3 &extA, const Vector3 &ba, const Vector3 &bb, const Vector3 &extB, float *entryTime, float *exitTime, Vector3 *normal);
	DLLMUTIL bool aabb_with_plane(const Vector3 &origin, const Vector3 &dir, const Vector3 &ext, const Vector3 &planeNormal, float planeDistance, float *t);
};

#endif // __VECTOR3_H__
