// SPDX-FileCopyrightText: (c) 2021 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

export module pragma.math:vertex;

export import :types;

export {
#pragma warning(push)
#pragma warning(disable : 4251)
	namespace pragma::math {
		constexpr double VERTEX_EPSILON = 0.001;

		struct DLLMUTIL Vertex {
			Vertex(const Vector3 &position, const Vector2 &uv, const Vector3 &normal, const Vector4 &tangent);
			Vertex(const Vector3 &position, const Vector2 &uv, const Vector3 &normal);
			Vertex(const Vector3 &position, const Vector3 &normal);
			Vertex();
			bool Equal(const Vertex &other, float epsilon) const;
			bool operator==(const Vertex &other) const;
			bool operator!=(const Vertex &other) const;
			Vector3 position = {};
			Vector2 uv = {};
			Vector3 normal = {};
			Vector4 tangent = {}; // w-component is handedness
			Vector3 GetBiTangent() const;
		};

		struct DLLMUTIL VertexWeight {
			VertexWeight(const Vector4i &boneIds, const Vector4 &weights);
			VertexWeight();
			bool operator==(const VertexWeight &other) const;
			bool operator!=(const VertexWeight &other) const;
			Vector4i boneIds = {};
			Vector4 weights = {};
		};

		DLLMUTIL std::ostream &operator<<(std::ostream &out, const Vertex &v);
		DLLMUTIL std::ostream &operator<<(std::ostream &out, const VertexWeight &v);
	};
#pragma warning(pop)
}
