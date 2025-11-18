// SPDX-FileCopyrightText: (c) 2019 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

module;

#include "definitions.hpp"

export module pragma.math:types;

export import glm;
export import std.compat;

export {
	using Mat2 = glm::mat2x2;
	using Mat2x2 = glm::mat2x2;
	using Mat2x3 = glm::mat2x3;
	using Mat2x4 = glm::mat2x4;
	using Mat3x2 = glm::mat3x2;
	using Mat3 = glm::mat3x3;
	using Mat3x3 = glm::mat3x3;
	using Mat3x4 = glm::mat3x4;
	using Mat4x2 = glm::mat4x2;
	using Mat4x3 = glm::mat4x3;
	using Mat4 = glm::mat4x4;
	using Mat4x4 = glm::mat4x4;

	using Vector2 = glm::vec2;
	using Vector3 = glm::vec3;
	using Vector4 = glm::vec4;

	using Vector2i = glm::ivec2;
	using Vector3i = glm::ivec3;
	using Vector4i = glm::ivec4;

	using Quat = glm::quat;

	using Candela = float;
	using Lumen = float;
	using Watt = float;
	using LuminousEfficacy = float; // Lumens per watt
	using Lux = float;
	using Irradiance = float; // Watts per square-meter
	using Kelvin = uint32_t;
	using Wavelength = uint32_t;

	namespace umath {
		using Bool = bool;

		using UInt = size_t;
		using Char = char;
		using UChar = unsigned char;
		using Int8 = int8_t;
		using UInt8 = uint8_t;
		using Int16 = int16_t;
		using UInt16 = uint16_t;
		using Int32 = int32_t;
		using UInt32 = uint32_t;
		using Int64 = int64_t;
		using UInt64 = uint64_t;

		using Float = float;
		using Double = double;
		using LDouble = long double;

		using Degree = float;
		using Radian = float;
		using Fraction = float; // Represents a value in the range [0,1]

		using Centimeter = float;
		using Meter = float;
		using Millimeter = float;
		using Kilometer = float;

		template<typename T>
		concept is_matrix_type = std::is_same_v<T, Mat2> || std::is_same_v<T, Mat2x3> || std::is_same_v<T, Mat2x4> || std::is_same_v<T, Mat3x2> || std::is_same_v<T, Mat3> || std::is_same_v<T, Mat3x4> || std::is_same_v<T, Mat4x2> || std::is_same_v<T, Mat4x3> || std::is_same_v<T, Mat4>;

		template<typename T>
		concept is_integral_vector_type = std::is_same_v<T, Vector2i> || std::is_same_v<T, Vector3i> || std::is_same_v<T, Vector4i>;

		template<typename T>
		concept is_floating_point_vector_type = std::is_same_v<T, Vector2> || std::is_same_v<T, Vector3> || std::is_same_v<T, Vector4>;

		template<typename T>
		concept is_vector_type = is_integral_vector_type<T> || is_floating_point_vector_type<T>;
	};

	using umath::Bool;
	using umath::Char;
	using umath::Double;
	using umath::Float;
	using umath::Int16;
	using umath::Int32;
	using umath::Int64;
	using umath::Int8;
	using umath::LDouble;
	using umath::UChar;
	using umath::UInt;
	using umath::UInt16;
	using umath::UInt32;
	using umath::UInt64;
	using umath::UInt8;

	template<typename T>
	constexpr Bool CBool(T value)
	{
		return static_cast<Bool>(value);
	}
	template<typename T>
	constexpr Char CChar(T value)
	{
		return static_cast<Char>(value);
	}
	template<typename T>
	constexpr UChar CUChar(T value)
	{
		return static_cast<UChar>(value);
	}
	template<typename T>
	constexpr Int8 CInt8(T value)
	{
		return static_cast<Int8>(value);
	}
	template<typename T>
	constexpr UInt8 CUInt8(T value)
	{
		return static_cast<UInt8>(value);
	}
	template<typename T>
	constexpr Int16 CInt16(T value)
	{
		return static_cast<Int16>(value);
	}
	template<typename T>
	constexpr UInt16 CUInt16(T value)
	{
		return static_cast<UInt16>(value);
	}
	template<typename T>
	constexpr Int32 CInt32(T value)
	{
		return static_cast<Int32>(value);
	}
	template<typename T>
	constexpr UInt32 CUInt32(T value)
	{
		return static_cast<UInt32>(value);
	}
	template<typename T>
	constexpr Int64 CInt64(T value)
	{
		return static_cast<Int64>(value);
	}
	template<typename T>
	constexpr UInt64 CUInt64(T value)
	{
		return static_cast<UInt64>(value);
	}
	template<typename T>
	constexpr Float CFloat(T value)
	{
		return static_cast<Float>(value);
	}
	template<typename T>
	constexpr Double CDouble(T value)
	{
		return static_cast<Double>(value);
	}
	template<typename T>
	constexpr LDouble CLDouble(T value)
	{
		return static_cast<LDouble>(value);
	}
	template<typename T>
	constexpr UInt CUInt(T value)
	{
		return static_cast<UInt>(value);
	}
}
