// SPDX-FileCopyrightText: (c) 2021 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

module;

module pragma.math;

import :vertex;

std::ostream &pragma::math::operator<<(std::ostream &out, const Vertex &v)
{
	out << "Vertex[" << v.position << "][" << v.normal << "][" << v.uv << "]";
	return out;
}

std::ostream &pragma::math::operator<<(std::ostream &out, const VertexWeight &v)
{
	out << "VertexWeight[" << v.boneIds << "][" << v.weights << "]";
	return out;
}

///////////////////////////////

pragma::math::Vertex::Vertex(const Vector3 &_position, const Vector2 &_uv, const Vector3 &_normal, const Vector4 &_tangent) : position(_position), uv(_uv), normal(_normal), tangent(_tangent) {}

pragma::math::Vertex::Vertex(const Vector3 &position, const Vector2 &uv, const Vector3 &normal) : Vertex(position, uv, normal, Vector4 {0.f, 0.f, 0.f, 1.f}) {}

pragma::math::Vertex::Vertex(const Vector3 &position, const Vector3 &normal) : Vertex(position, Vector2(0.f, 0.f), normal) {}

pragma::math::Vertex::Vertex() : Vertex(Vector3 {0.f, 0.f, 0.f}, Vector3 {0.f, 0.f, 0.f}) {}

Vector3 pragma::math::Vertex::GetBiTangent() const { return tangent.w * uvec::cross(normal, Vector3 {tangent}); }

bool pragma::math::Vertex::Equal(const Vertex &other, float epsilon) const
{
	return (math::abs(position.x - other.position.x) <= epsilon && math::abs(position.y - other.position.y) <= epsilon && math::abs(position.z - other.position.z) <= epsilon && math::abs(uv.x - other.uv.x) <= epsilon && math::abs(uv.y - other.uv.y) <= epsilon
	         && math::abs(normal.x - other.normal.x) <= epsilon && math::abs(normal.y - other.normal.y) <= epsilon && math::abs(normal.z - other.normal.z) <= epsilon)
	  ? true
	  : false;
}

bool pragma::math::Vertex::operator==(const Vertex &other) const { return Equal(other, VERTEX_EPSILON); }
bool pragma::math::Vertex::operator!=(const Vertex &other) const { return (*this == other) ? false : true; }

///////////////////////////////

pragma::math::VertexWeight::VertexWeight(const Vector4i &_boneIds, const Vector4 &_weights) : boneIds(_boneIds), weights(_weights) {}

pragma::math::VertexWeight::VertexWeight() : VertexWeight(Vector4i {-1, -1, -1, -1}, Vector4 {0.f, 0.f, 0.f, 0.f}) {}

bool pragma::math::VertexWeight::operator==(const VertexWeight &other) const
{
	std::array<bool, 4> matched {false};
	uint32_t numMatch = 0;
	auto n = boneIds.length();
	for(auto i = decltype(n) {0u}; i < n; ++i) {
		auto id0 = boneIds[i];
		auto weight0 = weights[i];
		for(auto j = decltype(n) {0u}; j < n; ++j) {
			auto id1 = other.boneIds[j];
			auto weight1 = other.weights[j];
			if(matched[j] || id0 != id1)
				continue;
			if(id0 != -1 && math::abs(weight0 - weight1) > VERTEX_EPSILON)
				break;
			matched[j] = true;
			++numMatch;
			break;
		}
	}
	return numMatch == n;
}
bool pragma::math::VertexWeight::operator!=(const VertexWeight &other) const { return (*this == other) ? false : true; }
