/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __MATHUTIL_VERTEX_HPP__
#define __MATHUTIL_VERTEX_HPP__

#include "mathutil/mathutildefinitions.h"
#include "mathutil/uvec.h"

#pragma warning(push)
#pragma warning(disable : 4251)
namespace umath
{
	static constexpr double VERTEX_EPSILON = 0.001;

	struct DLLMUTIL Vertex
	{
		Vertex(const Vector3 &position,const Vector2 &uv,const Vector3 &normal,const Vector4 &tangent);
		Vertex(const Vector3 &position,const Vector2 &uv,const Vector3 &normal);
		Vertex(const Vector3 &position,const Vector3 &normal);
		Vertex();
		bool Equal(const Vertex &other,float epsilon) const;
		bool operator==(const Vertex &other) const;
		bool operator!=(const Vertex &other) const;
		Vector3 position = {};
		Vector2 uv = {};
		Vector3 normal = {};
		Vector4 tangent = {}; // w-component is handedness
		Vector3 GetBiTangent() const;
	};

	struct DLLMUTIL VertexWeight
	{
		VertexWeight(const Vector4i &boneIds,const Vector4 &weights);
		VertexWeight();
		bool operator==(const VertexWeight &other) const;
		bool operator!=(const VertexWeight &other) const;
		Vector4i boneIds = {};
		Vector4 weights = {};
	};
};
#pragma warning(pop)

DLLMUTIL std::ostream &operator<<(std::ostream &out,const umath::Vertex &v);
DLLMUTIL std::ostream &operator<<(std::ostream &out,const umath::VertexWeight &v);

#endif
