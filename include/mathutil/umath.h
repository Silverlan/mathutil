/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __UMATH_H__
#define __UMATH_H__
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include "mathutildefinitions.h"
#include "scoped_enum_operators.hpp"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cmath>
#include <type_traits>
#include <limits>
#include <stdint.h>
#include <climits>
#include <vector>
#ifdef _WIN32
#include <Windows.h>
#else
#include <type_traits>
#endif

#undef min
#undef max

#define CBool(i) static_cast<Bool>(i)
#define CChar(i) static_cast<Char>(i)
#define CUChar(i) static_cast<UChar>(i)
#define CInt8(i) static_cast<Int8>(i)
#define CUInt8(i) static_cast<UInt8>(i)
#define CInt16(i) static_cast<Int16>(i)
#define CUInt16(i) static_cast<UInt16>(i)
#define CInt32(i) static_cast<Int32>(i)
#define CUInt32(i) static_cast<UInt32>(i)
#define CInt64(i) static_cast<Int64>(i)
#define CUInt64(i) static_cast<UInt64>(i)
#define CFloat(i) static_cast<Float>(i)
#define CDouble(i) static_cast<Double>(i)
#define CLDouble(i) static_cast<LDouble>(i)
#define CUInt(i) static_cast<UInt>(i)

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

	DLLMUTIL Int32 random(Int32 min, Int32 max);
	DLLMUTIL Float random(Float min, Float max);
	DLLMUTIL Double deg_to_rad(Double deg);
	DLLMUTIL Double rad_to_deg(Double rad);
	DLLMUTIL Double normalize_angle(Double ang, Double base);
	DLLMUTIL Double normalize_angle(Double ang);
	DLLMUTIL Double get_angle_difference(Double angA, Double angB);
	DLLMUTIL Double approach_angle(Double angA, Double angB, Double percent);
	template<typename T>
	constexpr T clamp(T val, T min, T max);
	template<typename T>
	T min(T a, T b);
	template<typename T, typename... Args>
	T min(T a, Args... args);
	template<typename T>
	T max(T a, T b);
	template<typename T, typename... Args>
	T max(T a, Args... args);

	template<class T>
	T pow(T base, T exp);
	template<class T>
	constexpr T pow2(T base);
	template<class T>
	constexpr T pow3(T base);
	template<class T>
	constexpr T pow4(T base);
	DLLMUTIL Double sin(Double x);
	DLLMUTIL Double cos(Double x);
	DLLMUTIL Double cot(Double x);
	DLLMUTIL Double tan(Double x);
	DLLMUTIL Double asin(Double x);
	DLLMUTIL Double acos(Double x);
	DLLMUTIL Double atan2(Double y, Double x);
	DLLMUTIL Double lerp(Double start, Double end, Double amount);
	DLLMUTIL Double lerp(Double v1, Double v2, Double i1, Double i2, Double x);
	DLLMUTIL Int32 ceil(Float f);
	DLLMUTIL Int32 ceil(Double d);
	DLLMUTIL Int64 ceil(LDouble d);
	DLLMUTIL Int32 floor(Float f);
	DLLMUTIL Int32 floor(Double d);
	DLLMUTIL Int64 floor(LDouble d);
	DLLMUTIL Int32 round(Float f);
	DLLMUTIL Int32 round(Double d);
	DLLMUTIL Int64 round(LDouble d);
	template<typename T>
	T round(T v, Int32 to)
	{
		T places = ::pow(10.0, T(to));
		return ::round(v * places) / places;
	}
	DLLMUTIL UInt64 next_power_of_2(UInt64 v);
	DLLMUTIL UInt64 previous_power_of_2(UInt64 v);
	DLLMUTIL std::vector<UInt64> get_power_of_2_values(UInt64 v);
	DLLMUTIL UInt32 get_number_of_decimals(Float f);
	template<class T>
	T abs(T v);
	template<class T>
	T approach(T val, T tgt, T inc);
	DLLMUTIL Int8 sign(Int32 i);
	DLLMUTIL Int8 sign(Float f);
	DLLMUTIL Int32 snap_to_grid(Float f, UInt32 gridSize = 1);
	DLLMUTIL Float snap_to_gridf(Float f, Float gridSize = 1);
	DLLMUTIL UInt8 get_highest_bit(UInt8 n);
	DLLMUTIL UInt16 get_highest_bit(UInt16 n);
	DLLMUTIL UInt32 get_highest_bit(UInt32 n);
	DLLMUTIL UInt64 get_highest_bit(UInt64 n);
	DLLMUTIL UInt32 get_least_significant_set_bit_index(UInt32 n);
	constexpr UInt32 get_least_significant_set_bit_index_c(UInt32 n)
	{
		// See https://github.com/stevengj/nlopt/blob/master/src/util/sobolseq.c#L87
		const uint32_t a = 0x05f66a47; /* magic number, found by brute force */
		const unsigned decode[32] = {0, 1, 2, 26, 23, 3, 15, 27, 24, 21, 19, 4, 12, 16, 28, 6, 31, 25, 22, 14, 20, 18, 11, 5, 30, 13, 17, 10, 29, 9, 8, 7};
#ifdef _MSC_VER
#pragma warning(suppress : 4146)
		n = a * (n & (-n)); /* same as below, but suppress spurious warning/error with MS compiler about negating unsigned int */
#else
		n = a * (n & (-n)); /* store in n to make sure mult. is 32 bits */
#endif
		return decode[n >> 27];
	}
	template<class T>
	T get_aligned_offset(T baseOffset, T alignment);

	template<typename E>
	constexpr typename std::underlying_type<E>::type to_integral(E e);

	template<class V, class U, typename T>
	typename std::enable_if<(std::is_integral<T>::value || std::is_floating_point<T>::value) && (std::is_integral<U>::value || std::is_floating_point<U>::value), U>::type limit(T in);
	template<class U, typename T>
	typename std::enable_if<std::is_integral<T>::value && std::is_integral<U>::value, U>::type limit(T in);
	template<class U, typename T>
	typename std::enable_if<std::is_floating_point<T>::value && std::is_floating_point<U>::value, U>::type limit(T in);

	template<typename A, typename B>
	void swap(A &a, B &b);
	template<typename T>
	void negate(T &t);

	// See https://en.wikipedia.org/wiki/Smoothstep for more information
	template<typename T>
	T smooth_step(T edge0, T edge1, T x);
	template<typename T>
	T smoother_step(T edge0, T edge1, T x);

	const auto pi = ::acos(-1);
	DLLMUTIL bool is_zero(double x);

	DLLMUTIL float sqrt(float v);
	DLLMUTIL double sqrt(double v);
	DLLMUTIL long double sqrt(long double v);
	DLLMUTIL Double lerp_angle(Double angA, Double angB, Double amount);
	DLLMUTIL Double clamp_angle(Double ang, Double min, Double max);
	DLLMUTIL bool is_angle_in_range(Double ang, Double min, Double max);
	// TODO: Replace these with the glm versions further below
	DLLMUTIL uint16_t float32_to_float16(float f);
	DLLMUTIL float float16_to_float32(uint16_t v);

	DLLMUTIL Radian calc_horizontal_fov(float focalLengthImMM, float width, float height);
	DLLMUTIL Radian calc_vertical_fov(float focalLengthImMM, float width, float height);
	DLLMUTIL Radian calc_diagonal_fov(float focalLengthImMM, float width, float height);

	DLLMUTIL Radian horizontal_fov_to_vertical_fov(Radian hFov, float width, float height);
	DLLMUTIL Radian vertical_fov_to_horizontal_fov(Radian vFov, float width, float height);
	DLLMUTIL Radian diagonal_fov_to_vertical_fov(Degree diagonalFov, float aspectRatio);

	DLLMUTIL float calc_dielectric_specular_reflection(float ior);

	DLLMUTIL float fade_in_out(float fadeInStart, float fadeInEnd, float fadeOutStart, float fadeOutEnd, float curTime);

	template<typename T>
	T abs_max(T a, T b);
	template<typename T, typename... Args>
	T abs_max(T a, Args... args);

	// Inclusive between
	template<typename T>
	bool between(T value, T min, T max);

	template<typename T>
	constexpr T get_number_of_times_dividable_by_x(T v, uint32_t x);

	template<typename T>
	T get_greatest_common_divisor(T a, T b);
	template<typename T>
	T get_least_common_multiple(T a, T b);

	DLLMUTIL int16_t float32_to_float16_glm(float f);
	DLLMUTIL float float16_to_float32_glm(int16_t v);

	template<typename T>
	constexpr void set_flag(T &baseFlags, T flag, bool set = true);
	template<typename T>
	constexpr void add_flag(T &baseFlags, T flag);
	template<typename T>
	constexpr void remove_flag(T &baseFlags, T flag);
	template<typename T>
	constexpr bool is_flag_set(T baseFlags, T flag);

	template<typename T>
	T swap_endian(T u);

	constexpr float calc_solid_angle(Radian apexAngleCos) { return 2 * M_PI * (1.0 - apexAngleCos); }
}

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

template<class T>
T umath::abs(T v)
{
	return (v < T(0)) ? -v : v;
}
template<class T>
T umath::approach(T val, T tgt, T inc)
{
	inc = abs(inc);
	if(val < tgt)
		val = min(val + inc, tgt);
	else
		val = max(val - inc, tgt);
	return val;
}

template<class T>
T umath::get_aligned_offset(T baseOffset, T alignment)
{
	static_assert(std::is_integral<T>::value, "Only integral types are allowed!");
	auto r = baseOffset % alignment;
	if(r == 0)
		return baseOffset;
	return baseOffset + alignment - r;
}

template<typename T>
constexpr T umath::clamp(T val, T min, T max)
{
	if(val < min)
		return min;
	if(val > max)
		return max;
	return val;
}

template<typename T>
T umath::min(T a, T b)
{
	return (a < b) ? a : b;
}

template<typename T, typename... Args>
T umath::min(T a, Args... args)
{
	return min(a, min(args...));
}

template<typename T>
T umath::max(T a, T b)
{
	return (a > b) ? a : b;
}

template<typename T, typename... Args>
T umath::max(T a, Args... args)
{
	return max(a, max(args...));
}

template<typename T>
T umath::abs_max(T a, T b)
{
	return (abs(a) > abs(b)) ? a : b;
}

template<typename T, typename... Args>
T umath::abs_max(T a, Args... args)
{
	return max(a, max(args...));
}

template<class T>
T umath::pow(T base, T exp)
{
	return ::pow(base, exp);
}

template<class T>
constexpr T umath::pow2(T base)
{
	return base * base;
}

template<class T>
constexpr T umath::pow3(T base)
{
	return pow2(base) * base;
}

template<class T>
constexpr T umath::pow4(T base)
{
	return pow3(base) * base;
}

template<typename E>
constexpr typename std::underlying_type<E>::type umath::to_integral(E e)
{
	return static_cast<typename std::underlying_type<E>::type>(e);
}

template<class V, class U, typename T>
typename std::enable_if<(std::is_integral<T>::value || std::is_floating_point<T>::value) && (std::is_integral<U>::value || std::is_floating_point<U>::value), U>::type umath::limit(T in)
{
	auto lIn = static_cast<V>(in);
	auto max = static_cast<V>(std::numeric_limits<U>::max());
	if(lIn > max)
		return static_cast<U>(max);
	auto min = static_cast<V>(std::numeric_limits<U>::lowest());
	if(lIn < min)
		return static_cast<U>(min);
	return static_cast<U>(lIn);
}

template<class U, typename T>
typename std::enable_if<std::is_integral<T>::value && std::is_integral<U>::value, U>::type umath::limit(T in)
{
	static_assert(!std::is_same<T, long long>::value, "Type not allowed: 'long long'!");
	static_assert(!std::is_same<T, signed long long>::value, "Type not allowed: 'signed long long'!");
	static_assert(!std::is_same<T, unsigned long long>::value, "Type not allowed: 'unsigned long long'!");

	static_assert(!std::is_same<U, unsigned long long>::value, "Type not allowed: 'unsigned long long'!");

	return umath::limit<long long, U, T>(in);
}

template<class U, typename T>
typename std::enable_if<std::is_floating_point<T>::value && std::is_floating_point<U>::value, U>::type umath::limit(T in)
{
	static_assert(!std::is_same<T, long double>::value, "Type not allowed: 'long double'!");
	static_assert(!std::is_same<U, long double>::value || !std::is_unsigned<U>::value, "Type not allowed: 'unsigned long double'!");

	return umath::limit<long double, U, T>(in);
}

template<typename A, typename B>
void umath::swap(A &a, B &b)
{
	auto tmp = a;
	a = b;
	b = tmp;
}

template<typename T>
void umath::negate(T &t)
{
	t = -t;
}

template<typename T>
T umath::smooth_step(T edge0, T edge1, T x)
{
	// Scale, bias and saturate x to 0..1 range
	x = clamp((x - edge0) / (edge1 - edge0), T(0), T(1));
	// Evaluate polynomial
	return x * x * (3 - 2 * x);
}
template<typename T>
T umath::smoother_step(T edge0, T edge1, T x)
{
	// Scale, and clamp x to 0..1 range
	x = clamp((x - edge0) / (edge1 - edge0), T(0), T(1));
	// Evaluate polynomial
	return x * x * x * (x * (x * 6 - 15) + 10);
}

template<typename T>
constexpr T umath::get_number_of_times_dividable_by_x(T v, uint32_t x)
{
	return ::floor(::log(v) / ::log(x));
}

template<typename T>
T umath::get_greatest_common_divisor(T a, T b)
{
	for(;;) {
		if(a == 0)
			return b;
		b %= a;
		if(b == 0)
			return a;
		a %= b;
	}
}
template<typename T>
T umath::get_least_common_multiple(T a, T b)
{
	auto temp = get_greatest_common_divisor(a, b);
	return temp ? (a / temp * b) : 0;
}

template<typename T>
constexpr void umath::set_flag(T &baseFlags, T flag, bool set)
{
	if(set)
		add_flag(baseFlags, flag);
	else
		remove_flag(baseFlags, flag);
}
template<typename T>
constexpr void umath::add_flag(T &baseFlags, T flag)
{
	// Messy msvc c++20 compiler bug fix
	reinterpret_cast<std::underlying_type_t<T> &>(baseFlags) |= static_cast<std::underlying_type_t<T>>(flag);
	// baseFlags |= flag;
}
template<typename T>
constexpr void umath::remove_flag(T &baseFlags, T flag)
{
	// Messy msvc c++20 compiler bug fix
	reinterpret_cast<std::underlying_type_t<T> &>(baseFlags) &= ~static_cast<std::underlying_type_t<T>>(flag);
	//baseFlags &= ~flag;
}

template<typename T>
constexpr bool umath::is_flag_set(T baseFlags, T flag)
{
	// Messy msvc c++20 compiler bug fix
	return (static_cast<std::underlying_type_t<T>>(baseFlags) & static_cast<std::underlying_type_t<T>>(flag)) != static_cast<std::underlying_type_t<T>>(0);
	//return static_cast<std::underlying_type_t<T>>(baseFlags &flag) != static_cast<std::underlying_type_t<T>>(0);
}

template<typename T>
T umath::swap_endian(T u)
{
	// Source: https://stackoverflow.com/a/4956493
	static_assert(CHAR_BIT == 8, "CHAR_BIT != 8");

	union {
		T u;
		unsigned char u8[sizeof(T)];
	} source, dest;

	source.u = u;

	for(size_t k = 0; k < sizeof(T); k++)
		dest.u8[k] = source.u8[sizeof(T) - k - 1];

	return dest.u;
}

template<typename T>
bool umath::between(T value, T min, T max)
{
	return value >= min && value <= max;
}

#endif
