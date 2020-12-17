/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mathutil/umath_geometry.hpp"

const float EPSILON = 1.19209e-005f;

static bool is_better_t(float tOld,float tNew)
{
	return tOld == std::numeric_limits<float>::max() || (tNew < 0.f && tNew > tOld) || (tNew >= 0.f && tNew < tOld);
}

static bool operator>(umath::intersection::Result a,umath::intersection::Result b)
{
	switch(a)
	{
		case umath::intersection::Result::NoIntersection:
			return b == umath::intersection::Result::Intersect || b == umath::intersection::Result::OutOfRange;
		case umath::intersection::Result::OutOfRange:
			return b == umath::intersection::Result::Intersect;
		case umath::intersection::Result::Intersect:
			return b != umath::intersection::Result::Intersect;
	}
	return false;
}

static bool operator>=(umath::intersection::Result a,umath::intersection::Result b) {return a > b || a == b;}
static bool operator<(umath::intersection::Result a,umath::intersection::Result b) {return !operator>=(a,b);}
static bool operator<=(umath::intersection::Result a,umath::intersection::Result b) {return a < b || a == b;}

bool umath::intersection::vector_in_bounds(const Vector3 &vec,const Vector3 &min,const Vector3 &max,float EPSILON)
{
	if(EPSILON == 0.f)
		return vec.x >= min.x && vec.y >= min.y && vec.z >= min.z
			&& vec.x <= max.x && vec.y <= max.y && vec.z <= max.z;
	return vec.x >= min.x -EPSILON && vec.y >= min.y -EPSILON && vec.z >= min.z -EPSILON
		&& vec.x <= max.x +EPSILON && vec.y <= max.y +EPSILON && vec.z <= max.z +EPSILON;
}

bool umath::intersection::sphere_sphere(const Vector3 &originA,float rA,const Vector3 &originB,float rB)
{
	float dist = glm::distance(originA,originB);
	return dist -(rA +rB) <= 0;
}

bool umath::intersection::aabb_sphere(const Vector3 &min,const Vector3 &max,const Vector3 &origin,float r)
{
	Vector3 pClosest;
	umath::geometry::closest_point_on_aabb_to_point(min,max,origin,&pClosest);
	float d = glm::distance2(pClosest,origin);
	return d <= r *r;
}

bool umath::intersection::aabb_in_aabb(const Vector3 &minA,const Vector3 &maxA,const Vector3 &minB,const Vector3 &maxB)
{
	return (((minA.x >= minB.x && minA.y >= minB.y && minA.z >= minB.z) &&
		(maxA.x <= maxB.x && maxA.y <= maxB.y && maxA.z <= maxB.z))) ? true : false;
}

bool umath::intersection::line_sphere(const Vector3 &lineOrigin,const Vector3 &lineDir,const Vector3 &sphereOrigin,float sphereRadius,float &outT,Vector3 &outP)
{
	// Source: https://gamedev.stackexchange.com/a/96487/49279
	auto m = lineOrigin -sphereOrigin;
	float b = uvec::dot(m,lineDir);
	float c = uvec::dot(m,m) -umath::pow2(sphereRadius);

	// Exit if r’s origin outside s (c > 0) and r pointing away from s (b > 0) 
	if(c > 0.0f && b > 0.0f)
		return false;
	float discr = b*b - c;

	// A negative discriminant corresponds to ray missing sphere 
	if(discr < 0.0f) return 0;

	// Ray now found to intersect sphere, compute smallest t value of intersection
	outT = -b - umath::sqrt(discr);

	// If t is negative, ray started inside sphere so clamp t to zero 
	if(outT < 0.0f) outT = 0.0f;
	outP = lineOrigin +outT *lineDir;
	return true;
}

umath::intersection::Intersect umath::intersection::aabb_aabb(const Vector3 &minA,const Vector3 &maxA,const Vector3 &minB,const Vector3 &maxB)
{
	if((maxA.x < minB.x) ||
			(minA.x > maxB.x) ||
			(maxA.y < minB.y) ||
			(minA.y > maxB.y) ||
			(maxA.z < minB.z) ||
			(minA.z > maxB.z)
		)
		return Intersect::Outside;
	if(aabb_in_aabb(minA,maxA,minB,maxB) || aabb_in_aabb(minB,maxB,minA,maxA))
		return Intersect::Inside;
	return Intersect::Overlap;
}

bool umath::intersection::aabb_aabb(const bounding_volume::AABB &a,const bounding_volume::AABB &b)
{
	Vector3 t = b.GetCenter() -a.GetCenter();
	auto extentsA = a.GetExtents();
	auto extentsB = b.GetExtents();
	return fabs(t.x) <= (extentsA.x +extentsB.x)
		&& fabs(t.y) <= (extentsA.y +extentsB.y)
		&& fabs(t.z) <= (extentsA.z +extentsB.z);
}

bool umath::intersection::aabb_triangle(const Vector3 &min,const Vector3 &max,const Vector3 &a,const Vector3 &b,const Vector3 &c)
{
	Vector3 center = (min +max) *0.5f;
	min -= center;
	max -= center;
	a -= center;
	b -= center;
	c -= center;
	Vector3 minTri = a;
	Vector3 maxTri = a;
	uvec::min(&minTri,b);
	uvec::min(&minTri,c);
	uvec::max(&maxTri,b);
	uvec::max(&maxTri,c);
	if(aabb_aabb(min,max,minTri,maxTri) == Intersect::Outside)
		return false;
	// TODO
	return true;
}

bool umath::intersection::obb_plane(const Vector3 &min,const Vector3 &max,const Vector3 &origin,const Quat &rot,const Vector3 &n,double d)
{
	const std::array<Vector3,8> points = {
		min,
		{max.x,min.y,min.z},
		{min.x,max.y,min.z},
		{min.x,min.y,max.z},
		{max.x,max.y,min.z},
		{min.x,max.y,max.z},
		{max.x,min.y,max.z},
		max
	};
	uint8_t hitFlags = 0;
	for(auto p : points)
	{
		uvec::rotate(&p,rot);
		p += origin;
		auto pProj = uvec::project_to_plane(p,n,d);
		auto dot = uvec::dot(n,p -pProj);
		if(dot < 0.f)
			hitFlags |= 1;
		else if(dot > 0.f)
			hitFlags |= 2;
		else
			return true; // Point is on plane
		if(hitFlags == (1 | 2))
			return true;
	}
	return false;
}
bool umath::intersection::aabb_plane(const Vector3 &min,const Vector3 &max,const Vector3 &n,double d) {return obb_plane(min,max,{},{},n,d);}
bool umath::intersection::sphere_plane(const Vector3 &sphereOrigin,float sphereRadius,const Vector3 &n,double d)
{
	auto pProj = uvec::project_to_plane(sphereOrigin,n,d);
	auto distSqr = uvec::length_sqr(sphereOrigin -pProj);
	return (distSqr <= umath::pow2(d)) ? true : false;
}

umath::intersection::Result umath::intersection::line_aabb(const Vector3 &o,const Vector3 &d,const Vector3 &min,const Vector3 &max,float *tMinRes,float *tMaxRes)
{
	Vector3 dirInv(1 /d.x,1 /d.y,1 /d.z);
	const int sign[] = {dirInv.x < 0,dirInv.y < 0,dirInv.z < 0};
	Vector3 bounds[] = {min,max};
	float tMin = (bounds[sign[0]].x -o.x) *dirInv.x;
	float tMax = (bounds[1 -sign[0]].x -o.x) *dirInv.x;
	float tyMin = (bounds[sign[1]].y -o.y) *dirInv.y;
	float tyMax = (bounds[1 -sign[1]].y -o.y) *dirInv.y;
	if((tMin > tyMax) || (tyMin > tMax))
		return Result::NoIntersection;
	if(tyMin > tMin)
		tMin = tyMin;
	if(tyMax < tMax)
		tMax = tyMax;
	float tzMin = (bounds[sign[2]].z -o.z) *dirInv.z;
	float tzMax = (bounds[1 -sign[2]].z -o.z) *dirInv.z;
	if((tMin > tzMax) || (tzMin > tMax))
		return Result::NoIntersection;
	if(tzMin > tMin)
		tMin = tzMin;
	if(tzMax < tMax)
		tMax = tzMax;
	if(tMinRes != nullptr)
		*tMinRes = tMin;
	if(tMaxRes != NULL)
		*tMaxRes = tMax;
	return Result::Intersect;
}

umath::intersection::Result umath::intersection::line_plane(const Vector3 &o,const Vector3 &dir,const Vector3 &nPlane,float distPlane,float *t)
{
	float f = glm::dot(nPlane,dir);
	if(f == 0.f)
		return Result::NoIntersection;
	float hit = (glm::dot(nPlane,(nPlane *-distPlane) -o)) /f;
	if(t != NULL)
		*t = hit;
	return (hit >= 0.f && hit <= 1.f) ? Result::Intersect : Result::OutOfRange;
}

static bool point_in_triangle(const Vector3 &p,const Vector3 &a,const Vector3 &b,const Vector3 &c,const Vector3 origin={},const Quat rot=uquat::identity())
{
	std::array<Vector3,3> tri = {a,b,c};
	for(auto &v : tri)
	{
		uvec::rotate(&v,rot);
		v += origin;

		v = p -v;
		uvec::normalize(&v);
	}
	auto angles = umath::acos(uvec::dot(tri[0],tri[1])) +umath::acos(uvec::dot(tri[1],tri[2])) +umath::acos(uvec::dot(tri[2],tri[0]));
	return (umath::abs(angles -2.f *M_PI) <= 0.005f) ? true : false;
}

bool umath::intersection::line_obb(const Vector3 &rayStart,const Vector3 &rayDir,const Vector3 &min,const Vector3 &max,float *dist,const Vector3 &origin,const Quat &rot)
{
	// Source: http://www.opengl-tutorial.org/miscellaneous/clicking-on-objects/picking-with-custom-ray-obb-function/
	auto tMin = 0.f;
	auto tMax = std::numeric_limits<float>::max();
	if(dist != nullptr)
		*dist = 0.f;

	auto t = umat::identity();
	t = glm::translate(t,origin);
	t *= umat::create(rot);

	auto obbPos = Vector3{t[3].x,t[3].y,t[3].z};
	auto delta = obbPos -rayStart;

	{
		Vector3 xaxis{t[0].x,t[0].y,t[0].z};
		auto e = uvec::dot(xaxis,delta);
		auto f = uvec::dot(rayDir,xaxis);

		if(umath::abs(f) > 0.001f)
		{
			auto t1 = (e +min.x) /f;
			auto t2 = (e +max.x) /f;

			if(t1 > t2)
			{
				auto w = t1;
				t1 = t2;
				t2 = w;
			}

			if(t2 < tMax)
				tMax = t2;
			if(t1 > tMin)
				tMin = t1;

			if(tMax < tMin)
				return false;
		}
		else if(-e +min.x > 0.f || -e +max.x < 0.f)
			return false;
	}

	{
		Vector3 yaxis{t[1].x,t[1].y,t[1].z};
		auto e = uvec::dot(yaxis,delta);
		auto f = uvec::dot(rayDir,yaxis);

		if(umath::abs(f) > 0.001f)
		{
			auto t1 = (e +min.y) /f;
			auto t2 = (e +max.y) /f;

			if(t1 > t2)
			{
				auto w = t1;
				t1 = t2;
				t2 = w;
			}

			if(t2 < tMax)
				tMax = t2;
			if(t1 > tMin)
				tMin = t1;
			if(tMin > tMax)
				return false;
		}
		else if(-e +min.y > 0.f || -e +max.y < 0.f)
			return false;
	}

	{
		Vector3 zaxis{t[2].x,t[2].y,t[2].z};
		auto e = uvec::dot(zaxis,delta);
		auto f = uvec::dot(rayDir,zaxis);

		if(umath::abs(f) > 0.001f)
		{
			auto t1 = (e +min.z) /f;
			auto t2 = (e +max.z) /f;

			if(t1 > t2)
			{
				auto w = t1;
				t1 = t2;
				t2 = w;
			}

			if(t2 < tMax)
				tMax = t2;
			if(t1 > tMin)
				tMin = t1;
			if(tMin > tMax)
				return false;
		}
		else if(-e +min.z > 0.f || -e +max.z < 0.f)
			return false;
	}
	if(dist != nullptr)
		*dist = tMin;
	return (tMin <= 1.f && tMin >= 0.f) ? true : false;
}

bool umath::intersection::point_in_plane_mesh(const Vector3 &vec,const std::vector<Plane> &planes)
{
	for(unsigned int i=0;i<planes.size();i++)
	{
		if(planes[i].GetDistance(vec) < 0.f)
			return false;
	}
	return true;
}

umath::intersection::Intersect umath::intersection::sphere_in_plane_mesh(const Vector3 &vec,float radius,const std::vector<Plane> &planes,bool skipInsideTest)
{
	if(point_in_plane_mesh(vec,planes) == false)
	{
		for(auto it=planes.begin();it!=planes.end();++it)
		{
			auto &plane = const_cast<Plane&>(*it);
			Vector3 p = vec +plane.GetNormal() *radius; // Closest point on sphere to plane
			if(plane.GetDistance(p) < 0)
				return Intersect::Outside;
		}
		return Intersect::Overlap;
	}
	if(skipInsideTest == true)
		return Intersect::Overlap;
	auto radiusSqr = umath::pow(radius,2.f);
	for(auto it=planes.begin();it!=planes.end();++it)
	{
		auto &plane = const_cast<Plane&>(*it);
		Vector3 r;
		umath::geometry::closest_point_on_plane_to_point(plane.GetNormal(),CFloat(-plane.GetDistance()),vec,&r);
		if(uvec::length_sqr(r -vec) < radiusSqr)
			return Intersect::Overlap;
	}
	return Intersect::Inside;
}

umath::intersection::Intersect umath::intersection::aabb_in_plane_mesh(const Vector3 &min,const Vector3 &max,const std::vector<Plane> &planes)
{
	// Note: If the current method causes problems, try switching to the other one.
	// The second method is faster for most cases.
#define AABB_PLANE_MESH_INTERSECTION_METHOD 1
#if AABB_PLANE_MESH_INTERSECTION_METHOD == 0
	// Source: https://www.gamedev.net/forums/topic/672043-perfect-aabb-frustum-intersection-test/?do=findComment&comment=5254253
	UInt result = INTERSECT_INSIDE;
	
	for(auto &plane : planes)
	{
		// planes have unit-length normal, offset = -dot(normal, point on plane)
		const auto &n = plane.GetNormal();
		auto d = plane.GetDistance();
		auto nx = n.x > double(0);
		auto ny = n.y > double(0);
		auto nz = n.z > double(0);
		
		// getMinMax(): 0 = return min coordinate. 1 = return max.
		auto getMinMax = [&min,&max](bool v) -> Vector3 {
			return v ? max : min;
		};
		auto dot = (n.x*getMinMax(nx).x) + (n.y*getMinMax(ny).y) + (n.z*getMinMax(nz).z);
		
		if ( dot < -d )
			return INTERSECT_OUTSIDE;
		
		auto dot2 = (n.x*getMinMax(1-nx).x) + (n.y*getMinMax(1-ny).y) + (n.z*getMinMax(1-nz).z);
		
		if ( dot2 <= -d )
			result = INTERSECT_OVERLAP;
	}
	
	return result;
#else
	Vector3 vMin,vMax;
	auto r = Intersect::Inside;
	for(auto &plane : planes)
	{
		auto &n = plane.GetNormal();
		if(n.x > 0)
		{
			vMin.x = min.x; 
			vMax.x = max.x; 
		}
		else
		{ 
			vMin.x = max.x; 
			vMax.x = min.x; 
		}
		if(n.y > 0)
		{ 
			vMin.y = min.y; 
			vMax.y = max.y; 
		}
		else
		{ 
			vMin.y = max.y; 
			vMax.y = min.y; 
		}
		if(n.z > 0)
		{ 
			vMin.z = min.z; 
			vMax.z = max.z; 
		}
		else
		{ 
			vMin.z = max.z; 
			vMax.z = min.z; 
		} 
		if(plane.GetDistance(vMax) < 0)
			return Intersect::Outside;
		else if(plane.GetDistance(vMin) < 0)
			r = Intersect::Overlap;
	}
	return r;
#endif
}

bool umath::intersection::sphere_cone(const Vector3 &sphereOrigin,float radius,const Vector3 &coneOrigin,const Vector3 &coneDir,float coneAngle,float coneSize)
{
	// Source: https://bartwronski.com/2017/04/13/cull-that-cone/
	auto V = sphereOrigin -coneOrigin;
	auto VlenSq = uvec::dot(V,V);
	auto V1len = uvec::dot(V,coneDir);
	auto distanceClosestPoint = cos(coneAngle) *sqrt(VlenSq -V1len *V1len) -V1len *sin(coneAngle);

	auto angleCull = distanceClosestPoint > radius;
	auto frontCull = V1len > radius +coneSize;
	auto backCull = V1len < -radius;
	return /*testSphere.w > 1 ||*/ !(angleCull || frontCull || backCull);
}

bool umath::intersection::sphere_cone(const Vector3 &sphereOrigin,float radius,const Vector3 &coneOrigin,const Vector3 &coneDir,float coneAngle)
{
	// Source: http://www.geometrictools.com/GTEngine/Include/Mathematics/GteIntrSphere3Cone3.h
	auto sinAngle = umath::sin(coneAngle);
	if(sinAngle == 0.f)
		return false;
	auto cosAngle = umath::cos(coneAngle);

	auto invSin = 1.f /sinAngle;
	auto cosSqr = cosAngle *cosAngle;
	auto CmV = sphereOrigin -coneOrigin;
	auto D = CmV +(radius *static_cast<float>(invSin)) *coneDir;
	auto lenSqr = uvec::dot(D,D);
	auto e = uvec::dot(D,coneDir);
	if(e > 0.f && e *e >= lenSqr *cosSqr)
	{
		auto sinSqr = sinAngle *sinAngle;
		lenSqr = uvec::dot(CmV,CmV);
		e = -uvec::dot(CmV,coneDir);
		if(e > 0.f && e *e >= lenSqr *sinSqr)
		{
			auto rSqr = radius *radius;
			return (lenSqr <= rSqr) ? true : false;
		}
		else
			return true;
	}
	return false;
}

// Source: http://www.cs.virginia.edu/~gfx/Courses/2003/ImageSynthesis/papers/Acceleration/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf
bool umath::intersection::line_triangle(const Vector3 &lineOrigin,const Vector3 &lineDir,const Vector3 &v0,const Vector3 &v1,const Vector3 &v2,double &t,double &u,double &v,bool bCull)
{
	const auto EPSILON = 0.000001;
	t = 0.0;
	u = 0.0;
	v = 0.0;
	auto edge1 = v1 -v0;
	auto edge2 = v2 -v0;
	auto pvec = uvec::cross(lineDir,edge2);

	auto det = uvec::dot(edge1,pvec);

	if(bCull == true)
	{
		if(det < EPSILON)
			return false;
		auto tvec = lineOrigin -v0;
		u = uvec::dot(tvec,pvec);
		if(u < 0.0 || u > det)
			return false;
		auto qvec = uvec::cross(tvec,edge1);
		v = uvec::dot(lineDir,qvec);
		if(v < 0.0 || (u +v) > det)
			return false;
		t = uvec::dot(edge2,qvec);
		auto invDet = 1.0 /det;
		t *= invDet;
		u *= invDet;
		v *= invDet;
	}
	else
	{
		if(det > -EPSILON && det < EPSILON)
			return false;
		auto invDet = 1.0 /det;
		auto tvec = lineOrigin -v0;
		u = uvec::dot(tvec,pvec) *invDet;
		if(u < 0.0 || u > 1.0)
			return false;
		auto qvec = uvec::cross(tvec,edge1);
		v = uvec::dot(lineDir,qvec) *invDet;
		if(v < 0.0 || (u +v) > 1.0)
			return false;
		t = uvec::dot(edge2,qvec) *invDet;
	}
	return true;
}

static float det(float a,float b,float c,float d)
{
	return a *d -b *c;
}

std::optional<Vector2> umath::intersection::line_line(const Vector2 &start0,const Vector2 &end0,const Vector2 &start1,const Vector2 &end1)
{
	// http://mathworld.wolfram.com/Line-LineIntersection.html
	auto detL1 = det(start0.x,start0.y,end0.x,end0.y);
	auto detL2 = det(start1.x,start1.y,end1.x,end1.y);
	auto x1mx2 = start0.x -end0.x;
	auto x3mx4 = start1.x -end1.x;
	auto y1my2 = start0.y -end0.y;
	auto y3my4 = start1.y -end1.y;

	auto xnom = det(detL1,x1mx2,detL2,x3mx4);
	auto ynom = det(detL1,y1my2,detL2,y3my4);
	auto denom = det(x1mx2,y1my2,x3mx4,y3my4);
	if(denom == 0.0) // Lines don't seem to cross
		return {};

	if(umath::abs(denom) < 0.01)
		return {};

	auto ixOut = xnom /denom;
	auto iyOut = ynom /denom;
	return Vector2{ixOut,iyOut};
}

////////////////////////////////////

void umath::geometry::closest_point_on_aabb_to_point(const Vector3 &min,const Vector3 &max,const Vector3 &point,Vector3 *res)
{
	for(int i=0;i<3;i++)
	{
		float v = point[i];
		if(v < min[i]) v = min[i];
		if(v > max[i]) v = max[i];
		(*res)[i] = v;
	}
}

void umath::geometry::closest_point_on_plane_to_point(const Vector3 &n,float d,const Vector3 &p,Vector3 *res)
{
	float t = glm::dot(n,p) -d;
	*res = p -t *n;
}

void umath::geometry::closest_point_on_triangle_to_point(const Vector3 &a,const Vector3 &b,const Vector3 &c,const Vector3 &p,Vector3 *res)
{
	Vector3 ab = b -a;
	Vector3 ac = c -a;
	Vector3 ap = p -a;
	float dA = glm::dot(ab,ap);
	float dB = glm::dot(ac,ap);
	if(dA <= 0.0f && dB <= 0.0f)
	{
		*res = a;
		return;
	}
	Vector3 bp = p -b;
	float dC = glm::dot(ab,bp);
	float dD = glm::dot(ac,bp);
	if(dC >= 0.0f && dD <= dC)
	{
		*res = b;
		return;
	}
	float vc = dA *dD -dC *dB;
	if(vc <= 0.0f && dA >= 0.0f && dC <= 0.0f)
	{
		float v = dA /(dA -dC);
		*res = a +v *ab;
		return;
	}
	Vector3 cp = p -c;
	float dE = glm::dot(ab,cp);
	float dF = glm::dot(ac,cp);
	if(dF >= 0.0f && dE <= dF)
	{
		*res = c;
		return;
	}
	float vb = dE *dB -dA *dF;
	if(vb <= 0.0f && dB >= 0.0f && dF <= 0.0f)
	{
		float w = dB /(dB -dF);
		*res = a +w *ac;
		return;
	}
	float va = dC *dF -dE *dD;
	if(va <= 0.0f && (dD -dC) >= 0.0f && (dE -dF) >= 0.0f)
	{
		float w = (dD -dC) /((dD -dC) +(dE -dF));
		*res = b +w *(c -b);
		return;
	}
	float denom = 1.0f /(va +vb +vc);
	float v = vb *denom;
	float w = vc *denom;
	*res = a +ab *v +ac *w;
}

float umath::geometry::closest_points_between_lines(const Vector3 &pA,const Vector3 &qA,const Vector3 &pB,const Vector3 &qB,float *s,float *t,Vector3 *cA,Vector3 *cB)
{
	Vector3 dA = qA -pA;
	Vector3 dB = qB -pB;
	Vector3 r = pA -pB;
	float a = glm::dot(dA,dA);
	float e = glm::dot(dB,dB);
	float f = glm::dot(dB,r);
	float &rs = *s;
	float &rt = *t;
	if(a <= EPSILON && e <= EPSILON)
	{
		rs = rt = 0.0f;
		*cA = pA;
		*cB = pB;
		return glm::dot(*cA -(*cB),*cA -(*cB));
	}
	if(a <= EPSILON)
	{
		rs = 0.0f;
		rt = f /e;
		rt = glm::clamp(rt,0.0f,1.0f);
	}
	else
	{
		float c = glm::dot(dA,r);
		if(e <= EPSILON)
		{
			rt = 0.0f;
			rs = glm::clamp(-c /a,0.0f,1.0f);
		}
		else
		{
			float b = glm::dot(dA,dB);
			float denom = a *e -b *b;
			if(denom != 0.0f)
				rs = glm::clamp((b *f -c *e) /denom,0.0f,1.0f);
			else
				rs = 0.0f;
			rt = (b *rs +f) /e;
			if(rt < 0.0f)
			{
				rt = 0.0f;
				rs = glm::clamp(-c /a,0.0f,1.0f);
			}
			else if(rt > 1.0f)
			{
				rt = 1.0f;
				rs = glm::clamp((b -c) /a,0.0f,1.0f);
			}
		}
	}
	*cA = pA +dA *rs;
	*cB = pB +dB *rt;
	return glm::dot(*cA -(*cB),*cA -(*cB));
}

Vector3 umath::geometry::closest_point_on_line_to_point(const Vector3 &start,const Vector3 &end,const Vector3 &p,bool bClampResultToSegment)
{
	auto ap = p -start;
	auto ab = end -start;
	auto lenSqr = uvec::length_sqr(ab);
	if(lenSqr == 0.f)
		return start;
	auto dotAbap = uvec::dot(ap,ab);
	auto dist = dotAbap /lenSqr;
	if(bClampResultToSegment == true)
	{
		if(dist < 0.f)
			return start;
		else if(dist > 1.f)
			return end;
	}
	return start +ab *dist;
}

Vector3 umath::geometry::closest_point_on_sphere_to_line(const Vector3 &origin,float radius,const Vector3 &start,const Vector3 &end,bool bClampResultToSegment)
{
	auto pLine = closest_point_on_line_to_point(start,end,origin,bClampResultToSegment);
	auto dir = pLine -origin;
	auto l = uvec::length(dir);
	if(l == 0.f)
		return origin;
	dir /= l;
	if(l < radius)
		radius = l;
	return origin +dir *radius;
}

////////////////////////////////////

bool umath::sweep::aabb_with_aabb(const Vector3 &aa,const Vector3 &ab,const Vector3 &extA,
	const Vector3 &ba,const Vector3 &bb,const Vector3 &extB,
	float *entryTime,float *exitTime,Vector3 *normal)
{
	bounding_volume::AABB a(aa,extA);
	bounding_volume::AABB b(ba,extB);
	*entryTime = 0;
	*exitTime = 0;
	if(normal != NULL)
	{
		normal->x = 0;
		normal->y = 0;
		normal->z = 0;
	}
	if(a.Intersects(b))
		return true;
	Vector3 va = ab -aa;
	Vector3 vb = bb -ba;
	Vector3 invEntry(0,0,0);
	Vector3 invExit(0,0,0);
	Vector3 entry(0,0,0);
	Vector3 exit(0,0,0);
	Vector3 aMax = aa +extA;
	Vector3 bMax = ba +extB;
	aa -= extA;
	ba -= extB;
	for(int i=0;i<3;i++)
	{
		if(va[i] > 0.0f)
		{
			invEntry[i] = ba[i] -aMax[i];
			invExit[i] = bMax[i] -aa[i];
		}
		else
		{
			invEntry[i] = bMax[i] -aa[i];
			invExit[i] = ba[i] -aMax[i];
		}
		if(va[i] == 0.0f)
		{
			entry[i] = -std::numeric_limits<float>::infinity();
			exit[i] = std::numeric_limits<float>::infinity();
		}
		else
		{
			entry[i] = invEntry[i] /va[i];
			exit[i] = invExit[i] /va[i];
		}
	}
	float tEntry = std::max(entry.x,std::max(entry.y,entry.z));
	float tExit = std::min(exit.x,std::min(exit.y,exit.z));
	if(tEntry > tExit || entry.x < 0 && entry.y < 0 && entry.z < 0 || entry.x > 1 || entry.y > 1 || entry.z > 1)
		return false;
	if(entry.x > entry.y && entry.x > entry.z)
	{
		if(invEntry.x < 0)
		{
			normal->x = 1;
			normal->y = 0;
			normal->z = 0;
		}
		else
		{
			normal->x = -1;
			normal->y = 0;
			normal->z = 0;
		}
	}
	else if(entry.y > entry.x && entry.y > entry.z)
	{
		if(invEntry.y < 0)
		{
			normal->x = 0;
			normal->y = 1;
			normal->z = 0;
		}
		else
		{
			normal->x = 0;
			normal->y = -1;
			normal->z = 0;
		}
	}
	else
	{
		if(invEntry.z < 0)
		{
			normal->x = 0;
			normal->y = 0;
			normal->z = 1;
		}
		else
		{
			normal->x = 0;
			normal->y = 0;
			normal->z = -1;
		}
	}
	*entryTime = tEntry;
	*exitTime = tExit;
	return true;
}

bool umath::sweep::aabb_with_plane(const Vector3 &origin,const Vector3 &dir,const Vector3 &ext,const Vector3 &planeNormal,float planeDistance,float *t)
{
	float r = ext.x *fabs(planeNormal.x) +ext.y *fabs(planeNormal.y) +ext.z *fabs(planeNormal.z);
	*t = (r -planeDistance -(glm::dot(planeNormal,origin))) /glm::dot(planeNormal,dir);
	if(*t < 0 || *t > 1)
		return false;
	return true;
}
