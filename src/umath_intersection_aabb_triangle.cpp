// SPDX-FileCopyrightText: (c) 2022 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

#include "mathutil/umath_geometry.hpp"
#include "mathutil/transform.hpp"

static bool triBoxOverlap(glm::vec3 boxcenter, glm::vec3 boxhalfsize, glm::vec3 tv0, glm::vec3 tv1, glm::vec3 tv2);

bool umath::intersection::aabb_triangle(const Vector3 &pmin, const Vector3 &pmax, const Vector3 &pa, const Vector3 &pb, const Vector3 &pc)
{
	auto center = (pmin + pmax) * 0.5f;
	auto extents = (pmax - pmin) * 0.5f;
	return triBoxOverlap(center, extents, pa, pb, pc);
}

bool umath::intersection::obb_triangle(const Vector3 &min, const Vector3 &max, const ScaledTransform &transform, const Vector3 &a, const Vector3 &b, const Vector3 &c)
{

	auto tInv = transform.GetInverse();
	return aabb_triangle(min, max, tInv * a, tInv * b, tInv * c);
}

bool umath::intersection::obb_triangle(const Vector3 &min, const Vector3 &max, const Vector3 &origin, const Quat &rot, const Vector3 &a, const Vector3 &b, const Vector3 &c)
{
	umath::Transform t {origin, rot};
	t = t.GetInverse();
	return aabb_triangle(min, max, t * a, t * b, t * c);
}

// Source: https://gist.github.com/jflipts/fc68d4eeacfcc04fbdb2bf38e0911850
static void findMinMax(float x0, float x1, float x2, float &min, float &max)
{
	min = max = x0;
	if(x1 < min)
		min = x1;
	if(x1 > max)
		max = x1;
	if(x2 < min)
		min = x2;
	if(x2 > max)
		max = x2;
}

static bool planeBoxOverlap(glm::vec3 normal, glm::vec3 vert, glm::vec3 maxbox)
{
	glm::vec3 vmin, vmax;
	float v;
	for(size_t q = 0; q < 3; q++) {
		v = vert[q];
		if(normal[q] > 0.0f) {
			vmin[q] = -maxbox[q] - v;
			vmax[q] = maxbox[q] - v;
		}
		else {
			vmin[q] = maxbox[q] - v;
			vmax[q] = -maxbox[q] - v;
		}
	}
	if(glm::dot(normal, vmin) > 0.0f)
		return false;
	if(glm::dot(normal, vmax) >= 0.0f)
		return true;

	return false;
}

/*======================== X-tests ========================*/

static bool axisTestX01(float a, float b, float fa, float fb, const glm::vec3 &v0, const glm::vec3 &v2, const glm::vec3 &boxhalfsize, float &rad, float &min, float &max, float &p0, float &p2)
{
	p0 = a * v0.y - b * v0.z;
	p2 = a * v2.y - b * v2.z;
	if(p0 < p2) {
		min = p0;
		max = p2;
	}
	else {
		min = p2;
		max = p0;
	}
	rad = fa * boxhalfsize.y + fb * boxhalfsize.z;
	if(min > rad || max < -rad)
		return false;
	return true;
}
static bool axisTestX2(float a, float b, float fa, float fb, const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &boxhalfsize, float &rad, float &min, float &max, float &p0, float &p1)
{
	p0 = a * v0.y - b * v0.z;
	p1 = a * v1.y - b * v1.z;
	if(p0 < p1) {
		min = p0;
		max = p1;
	}
	else {
		min = p1;
		max = p0;
	}
	rad = fa * boxhalfsize.y + fb * boxhalfsize.z;
	if(min > rad || max < -rad)
		return false;
	return true;
}

/*======================== Y-tests ========================*/

static bool axisTestY02(float a, float b, float fa, float fb, const glm::vec3 &v0, const glm::vec3 &v2, const glm::vec3 &boxhalfsize, float &rad, float &min, float &max, float &p0, float &p2)
{
	p0 = -a * v0.x + b * v0.z;
	p2 = -a * v2.x + b * v2.z;
	if(p0 < p2) {
		min = p0;
		max = p2;
	}
	else {
		min = p2;
		max = p0;
	}
	rad = fa * boxhalfsize.x + fb * boxhalfsize.z;
	if(min > rad || max < -rad)
		return false;
	return true;
}

static bool axisTestY1(float a, float b, float fa, float fb, const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &boxhalfsize, float &rad, float &min, float &max, float &p0, float &p1)
{
	p0 = -a * v0.x + b * v0.z;
	p1 = -a * v1.x + b * v1.z;
	if(p0 < p1) {
		min = p0;
		max = p1;
	}
	else {
		min = p1;
		max = p0;
	}
	rad = fa * boxhalfsize.x + fb * boxhalfsize.z;
	if(min > rad || max < -rad)
		return false;
	return true;
}

/*======================== Z-tests ========================*/
static bool axisTestZ12(float a, float b, float fa, float fb, const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &boxhalfsize, float &rad, float &min, float &max, float &p1, float &p2)
{
	p1 = a * v1.x - b * v1.y;
	p2 = a * v2.x - b * v2.y;
	if(p1 < p2) {
		min = p1;
		max = p2;
	}
	else {
		min = p2;
		max = p1;
	}
	rad = fa * boxhalfsize.x + fb * boxhalfsize.y;
	if(min > rad || max < -rad)
		return false;
	return true;
}

static bool axisTestZ0(float a, float b, float fa, float fb, const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &boxhalfsize, float &rad, float &min, float &max, float &p0, float &p1)
{
	p0 = a * v0.x - b * v0.y;
	p1 = a * v1.x - b * v1.y;
	if(p0 < p1) {
		min = p0;
		max = p1;
	}
	else {
		min = p1;
		max = p0;
	}
	rad = fa * boxhalfsize.x + fb * boxhalfsize.y;
	if(min > rad || max < -rad)
		return false;
	return true;
}

bool triBoxOverlap(glm::vec3 boxcenter, glm::vec3 boxhalfsize, glm::vec3 tv0, glm::vec3 tv1, glm::vec3 tv2)
{
	/*    use separating axis theorem to test overlap between triangle and box */
	/*    need to test for overlap in these directions: */
	/*    1) the {x,y,z}-directions (actually, since we use the AABB of the triangle */
	/*       we do not even need to test these) */
	/*    2) normal of the triangle */
	/*    3) crossproduct(edge from tri, {x,y,z}-directin) */
	/*       this gives 3x3=9 more tests */
	glm::vec3 v0, v1, v2;
	float min, max, p0, p1, p2, rad, fex, fey, fez;
	glm::vec3 normal, e0, e1, e2;

	/* This is the fastest branch on Sun */
	/* move everything so that the boxcenter is in (0,0,0) */
	v0 = tv0 - boxcenter;
	v1 = tv1 - boxcenter;
	v2 = tv2 - boxcenter;

	/* compute triangle edges */
	e0 = v1 - v0;
	e1 = v2 - v1;
	e2 = v0 - v2;

	/* Bullet 3:  */
	/*  test the 9 tests first (this was faster) */
	fex = fabsf(e0.x);
	fey = fabsf(e0.y);
	fez = fabsf(e0.z);

	if(!axisTestX01(e0.z, e0.y, fez, fey, v0, v2, boxhalfsize, rad, min, max, p0, p2))
		return false;
	if(!axisTestY02(e0.z, e0.x, fez, fex, v0, v2, boxhalfsize, rad, min, max, p0, p2))
		return false;
	if(!axisTestZ12(e0.y, e0.x, fey, fex, v1, v2, boxhalfsize, rad, min, max, p1, p2))
		return false;

	fex = fabsf(e1.x);
	fey = fabsf(e1.y);
	fez = fabsf(e1.z);

	if(!axisTestX01(e1.z, e1.y, fez, fey, v0, v2, boxhalfsize, rad, min, max, p0, p2))
		return false;
	if(!axisTestY02(e1.z, e1.x, fez, fex, v0, v2, boxhalfsize, rad, min, max, p0, p2))
		return false;
	if(!axisTestZ0(e1.y, e1.x, fey, fex, v0, v1, boxhalfsize, rad, min, max, p0, p1))
		return false;

	fex = fabsf(e2.x);
	fey = fabsf(e2.y);
	fez = fabsf(e2.z);
	if(!axisTestX2(e2.z, e2.y, fez, fey, v0, v1, boxhalfsize, rad, min, max, p0, p1))
		return false;
	if(!axisTestY1(e2.z, e2.x, fez, fex, v0, v1, boxhalfsize, rad, min, max, p0, p1))
		return false;
	if(!axisTestZ12(e2.y, e2.x, fey, fex, v1, v2, boxhalfsize, rad, min, max, p1, p2))
		return false;

	/* Bullet 1: */
	/*  first test overlap in the {x,y,z}-directions */
	/*  find min, max of the triangle each direction, and test for overlap in */
	/*  that direction -- this is equivalent to testing a minimal AABB around */
	/*  the triangle against the AABB */

	/* test in X-direction */
	findMinMax(v0.x, v1.x, v2.x, min, max);
	if(min > boxhalfsize.x || max < -boxhalfsize.x)
		return false;

	/* test in Y-direction */
	findMinMax(v0.y, v1.y, v2.y, min, max);
	if(min > boxhalfsize.y || max < -boxhalfsize.y)
		return false;

	/* test in Z-direction */
	findMinMax(v0.z, v1.z, v2.z, min, max);
	if(min > boxhalfsize.z || max < -boxhalfsize.z)
		return false;

	/* Bullet 2: */
	/*  test if the box intersects the plane of the triangle */
	/*  compute plane equation of triangle: normal*x+d=0 */
	normal = glm::cross(e0, e1);
	if(!planeBoxOverlap(normal, v0, boxhalfsize))
		return false;

	return true; /* box and triangle overlaps */
}
