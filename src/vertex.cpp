/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mathutil/vertex.hpp"

std::ostream &operator<<(std::ostream &out,const umath::Vertex &v)
{
	out<<"Vertex["<<v.position<<"]["<<v.normal<<"]["<<v.uv<<"]";
	return out;
}

std::ostream &operator<<(std::ostream &out,const umath::VertexWeight &v)
{
	out<<"VertexWeight["<<v.boneIds<<"]["<<v.weights<<"]";
	return out;
}

///////////////////////////////

umath::Vertex::Vertex(const Vector3 &_position,const Vector2 &_uv,const Vector3 &_normal,const Vector4 &_tangent)
	: position(_position),uv(_uv),normal(_normal),tangent(_tangent)
{}

umath::Vertex::Vertex(const Vector3 &position,const Vector2 &uv,const Vector3 &normal)
	: Vertex(position,uv,normal,Vector4{0.f,0.f,0.f,1.f})
{}

umath::Vertex::Vertex(const Vector3 &position,const Vector3 &normal)
	: Vertex(position,Vector2(0.f,0.f),normal)
{}

umath::Vertex::Vertex()
	: Vertex(Vector3{0.f,0.f,0.f},Vector3{0.f,0.f,0.f})
{}

Vector3 umath::Vertex::GetBiTangent() const
{
	return tangent.w *uvec::cross(normal,Vector3{tangent});
}

bool umath::Vertex::Equal(const Vertex &other,float epsilon) const
{
	return (
		umath::abs(position.x -other.position.x) <= epsilon && umath::abs(position.y -other.position.y) <= epsilon && umath::abs(position.z -other.position.z) <= epsilon &&
		umath::abs(uv.x -other.uv.x) <= epsilon && umath::abs(uv.y -other.uv.y) <= epsilon &&
		umath::abs(normal.x -other.normal.x) <= epsilon && umath::abs(normal.y -other.normal.y) <= epsilon && umath::abs(normal.z -other.normal.z) <= epsilon
	) ? true : false;
}

bool umath::Vertex::operator==(const Vertex &other) const {return Equal(other,VERTEX_EPSILON);}
bool umath::Vertex::operator!=(const Vertex &other) const
{
	return (*this == other) ? false : true;
}

///////////////////////////////

umath::VertexWeight::VertexWeight(const Vector4i &_boneIds,const Vector4 &_weights)
	: boneIds(_boneIds),weights(_weights)
{}

umath::VertexWeight::VertexWeight()
	: VertexWeight(Vector4i{-1,-1,-1,-1},Vector4{0.f,0.f,0.f,0.f})
{}

bool umath::VertexWeight::operator==(const VertexWeight &other) const
{
	std::array<bool,4> matched {false};
	uint32_t numMatch = 0;
	auto n = boneIds.length();
	for(auto i=decltype(n){0u};i<n;++i)
	{
		auto id0 = boneIds[i];
		auto weight0 = weights[i];
		for(auto j=decltype(n){0u};j<n;++j)
		{
			auto id1 = other.boneIds[j];
			auto weight1 = other.weights[j];
			if(matched[j] || id0 != id1)
				continue;
			if(id0 != -1 && umath::abs(weight0 -weight1) > VERTEX_EPSILON)
				break;
			matched[j] = true;
			++numMatch;
			break;
		}
	}
	return numMatch == n;
}
bool umath::VertexWeight::operator!=(const VertexWeight &other) const
{
	return (*this == other) ? false : true;
}
