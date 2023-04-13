/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mathutil/umath.h"
#include "mathutil/umath_float16_compressor.h"
#include "mathutil/umath_random.hpp"
#include <string>
#include <glm/glm.hpp>
#include <glm/detail/type_half.hpp>

#define SMODF(a, n) fmod((fmod(a, n) + n), n)

static std::random_device random_device;
static std::mt19937 random_generator(random_device());
std::mt19937 &umath::get_default_random_generator() { return random_generator; }

/* epsilon surrounding for near zero values */
const auto EQN_EPS = 1e-9;
bool umath::is_zero(double x) { return ((x > -EQN_EPS && x < EQN_EPS)) ? true : false; }

Int32 umath::random(Int32 min, Int32 max)
{
	if(max < min)
		return random(max, min);
	std::uniform_int_distribution<Int32> dis(min, max);
	return dis(random_generator);
}

Float umath::random(Float min, Float max)
{
	if(max < min)
		return random(max, min);
	std::uniform_real_distribution<Float> dis(min, max);
	return dis(random_generator);
}

Double umath::deg_to_rad(Double deg) { return (deg / 180.0) * Double(M_PI); }
Double umath::rad_to_deg(Double rad) { return (rad * 180.0) / Double(M_PI); }

Double umath::normalize_angle(Double ang, Double base)
{
	Double max = base + 360;
	while(ang > max)
		ang -= 360;
	while(ang <= base)
		ang += 360;
	return ang;
}

Double umath::normalize_angle(Double ang)
{
	while(ang > 180)
		ang -= 360;
	while(ang <= -180)
		ang += 360;
	return ang;
}

Double umath::sin(Double x) { return ::sin(x); }
Double umath::cos(Double x) { return ::cos(x); }
Double umath::cot(Double x) { return cos(x) / sin(x); }
Double umath::tan(Double x) { return ::tan(x); }
Double umath::asin(Double x)
{
	if(x > 1.0f)
		x = 1.0f;
	else if(x < -1.0f)
		x = -1.0f;
	return ::asin(x);
}
Double umath::acos(Double x)
{
	if(x > 1.0f)
		x = 1.0f;
	else if(x < -1.0f)
		x = -1.0f;
	return ::acos(x);
}
Double umath::atan2(Double y, Double x)
{
	if(y == 0.0f && x == 0.0f)
		return 0.0f;
	return ::atan2(y, x);
}
Double umath::lerp(Double start, Double end, Double percent) { return (start + percent * (end - start)); }
Double umath::lerp(Double v1, Double v2, Double i1, Double i2, Double x) { return v1 + (v2 - v1) * (x - i1) / (i2 - i1); }
Double umath::lerp_angle(Double angA, Double angB, Double amount)
{
	amount = get_angle_difference(angA, angB) * amount;
	return approach_angle(angA, angB, amount);
}
Double umath::clamp_angle(Double ang, Double min, Double max)
{
	auto nMin = normalize_angle(min - ang);
	auto nMax = normalize_angle(max - ang);

	if(nMin <= 0.f && nMax >= 0.f)
		return ang;
	if(abs(nMin) < abs(nMax))
		return min;
	return max;
}
bool umath::is_angle_in_range(Double ang, Double min, Double max)
{
	auto nMin = normalize_angle(min - ang);
	auto nMax = normalize_angle(max - ang);
	return (nMin <= 0.f && nMax >= 0.f) ? true : false;
}
Double umath::get_angle_difference(Double angA, Double angB) { return SMODF((angB - angA) + 180.f, 360.f) - 180.f; }
Double umath::approach_angle(Double angA, Double angB, Double amount)
{
	Double diff = get_angle_difference(angA, angB);
	if(diff > amount)
		diff = amount;
	else if(diff < -amount)
		diff = -amount;
	return angA + diff;
}

Int32 umath::ceil(Float f) { return static_cast<Int32>(::ceilf(f)); }
Int32 umath::ceil(Double d) { return static_cast<Int32>(::ceil(d)); }
Int64 umath::ceil(LDouble d) { return static_cast<Int64>(::ceill(d)); }
Int32 umath::floor(Float f) { return static_cast<Int32>(::floorf(f)); }
Int32 umath::floor(Double d) { return static_cast<Int32>(::floor(d)); }
Int64 umath::floor(LDouble d) { return static_cast<Int64>(::floorl(d)); }
Int32 umath::round(Float f) { return static_cast<Int32>(::roundf(f)); }
Int32 umath::round(Double d) { return static_cast<Int32>(::round(d)); }
Int64 umath::round(LDouble d) { return static_cast<Int64>(::roundl(d)); }

UInt64 umath::next_power_of_2(UInt64 v)
{
	UInt64 powerOf2 = 1;
	if(v > 0) {
		while(v) {
			v >>= 1;
			powerOf2 <<= 1;
		}
	}
	return powerOf2;
}

UInt64 umath::previous_power_of_2(UInt64 v)
{
	UInt64 powerOf2 = 1;
	if(v > 1) {
		v--;
		while(v) {
			v >>= 1;
			powerOf2 <<= 1;
		}
		powerOf2 >>= 1;
	}
	return powerOf2;
}

std::vector<UInt64> umath::get_power_of_2_values(UInt64 v)
{
	std::vector<UInt64> values;
	auto next = next_power_of_2(v) / 2;
	while(v > 0 && next > 0) {
		if((v & next) != 0) {
			values.push_back(next);
			v &= ~next;
		}
		next /= 2;
	}
	return values;
}

UInt32 umath::get_number_of_decimals(Float f)
{
	f -= floor(f);
	auto str = std::to_string(f);
	auto p = str.find_first_of('.');
	if(p == size_t(-1))
		return 0;
	auto len = str.length();
	UInt32 numTrailingZeroes = 0;
	while(len > 0 && str[len - 1] == '0') {
		numTrailingZeroes++;
		len--;
	}
	str = str.substr(0, str.length() - numTrailingZeroes);
	return CUInt32(str.length() - p - 1);
}

Int8 umath::sign(Int32 i) { return (i >= 0) ? 1 : -1; }
Int8 umath::sign(Float f) { return (f >= 0) ? 1 : -1; }

Int32 umath::snap_to_grid(Float f, UInt32 gridSize)
{
	if(gridSize == 0)
		return 0;
	auto s = sign(f);
	auto r = static_cast<Int32>(f * s);
	auto d = r % gridSize;
	if(d < (CFloat(gridSize) * 0.5f))
		r -= d;
	else
		r += gridSize - d;
	return r * s;
}

float umath::snap_to_gridf(float f, float gridSize)
{
	if(gridSize == 0)
		return 0;
	auto s = umath::sign(f);
	auto r = f * s;
	auto d = fmodf(r, gridSize);
	if(d < (CFloat(gridSize) * 0.5f))
		r -= d;
	else
		r += gridSize - d;
	return r * s;
}

UInt8 umath::get_highest_bit(UInt8 n)
{
	n |= (n >> 1);
	n |= (n >> 2);
	n |= (n >> 4);
	return n - (n >> 1);
}
UInt16 umath::get_highest_bit(UInt16 n)
{
	n |= (n >> 1);
	n |= (n >> 2);
	n |= (n >> 4);
	n |= (n >> 8);
	return n - (n >> 1);
}
UInt32 umath::get_highest_bit(UInt32 n)
{
	n |= (n >> 1);
	n |= (n >> 2);
	n |= (n >> 4);
	n |= (n >> 8);
	n |= (n >> 16);
	return n - (n >> 1);
}
UInt64 umath::get_highest_bit(UInt64 n)
{
	n |= (n >> 1);
	n |= (n >> 2);
	n |= (n >> 4);
	n |= (n >> 8);
	n |= (n >> 16);
	n |= (n >> 32);
	return n - (n >> 1);
}
/* Return position (0, 1, ...) of rightmost (least-significant) one bit in n.
 *
 * This code uses a 32-bit version of algorithm to find the rightmost
 * one bit in Knuth, _The Art of Computer Programming_, volume 4A
 * (draft fascicle), section 7.1.3, "Bitwise tricks and
 * techniques." 
 *
 * Assumes n has a 1 bit, i.e. n != 0
 *
 */
// Source: https://stackoverflow.com/a/61701395/2482983
UInt32 umath::get_least_significant_set_bit_index(UInt32 n)
{
	const uint32_t a = 0x05f66a47; /* magic number, found by brute force */
	static const unsigned decode[32] = {0, 1, 2, 26, 23, 3, 15, 27, 24, 21, 19, 4, 12, 16, 28, 6, 31, 25, 22, 14, 20, 18, 11, 5, 30, 13, 17, 10, 29, 9, 8, 7};
	n = a * (n & (-n));
	return decode[n >> 27];
}

float umath::sqrt(float v) { return sqrtf(v); }
double umath::sqrt(double v) { return ::sqrt(v); }
long double umath::sqrt(long double v) { return sqrtl(v); }

uint16_t umath::float32_to_float16(float f) { return Float16Compressor::compress(f); }
float umath::float16_to_float32(uint16_t v) { return Float16Compressor::decompress(v); }

int16_t umath::float32_to_float16_glm(float f) { return glm::detail::toFloat16(f); }
float umath::float16_to_float32_glm(int16_t v) { return glm::detail::toFloat32(v); }

umath::Radian umath::calc_horizontal_fov(float focalLengthImMM, float width, float height) { return 2.f * atan2(width / 2.f, focalLengthImMM); }
umath::Radian umath::calc_vertical_fov(float focalLengthImMM, float width, float height) { return 2.f * atan2(height / 2.f, focalLengthImMM); }
umath::Radian umath::calc_diagonal_fov(float focalLengthImMM, float width, float height) { return 2.f * atan2(sqrt(pow2(width) + pow2(height)) / 2.f, focalLengthImMM); }
umath::Radian umath::horizontal_fov_to_vertical_fov(Radian hFov, float width, float height) { return 2.f * atanf(tanf(hFov / 2.f) * (height / width)); }
umath::Radian umath::vertical_fov_to_horizontal_fov(Radian vFov, float width, float height) { return 2.f * atanf(tanf(vFov / 2.f) * (width / height)); }
umath::Radian umath::diagonal_fov_to_vertical_fov(Degree diagonalFov, float aspectRatio)
{
	auto diagonalRatio = sqrt(1.f + pow2(aspectRatio));
	return (atan(tan(diagonalFov * (umath::deg_to_rad(1.f) / 2.f)) / diagonalRatio)) * umath::rad_to_deg(1.f) * 2.f;
}
float umath::calc_dielectric_specular_reflection(float ior)
{
	return umath::pow2(((ior - 1.0) / (ior + 1.0))) / 0.08; // Source: https://docs.blender.org/manual/en/latest/render/shader_nodes/shader/principled.html
}
float umath::fade_in_out(float fadeInStart, float fadeInEnd, float fadeOutStart, float fadeOutEnd, float curTime)
{
	if(fadeInStart > curTime)
		return 0.0;

	if((fadeOutEnd > 0.f) && (fadeOutEnd < curTime))
		return 0.f;

	fadeInEnd = max(fadeInEnd, fadeInStart);
	fadeOutStart = max(fadeOutStart, fadeInEnd);
	fadeOutEnd = max(fadeOutEnd, fadeOutStart);

	auto flStrength = 1.f;
	if((fadeInEnd > curTime) && (fadeInEnd > fadeInStart))
		flStrength = min(flStrength, static_cast<float>(lerp(0, 1, fadeInStart, fadeInEnd, curTime)));

	if((curTime > fadeOutStart) && (fadeOutEnd > fadeOutStart))
		flStrength = min(flStrength, static_cast<float>(lerp(0, 1, fadeOutEnd, fadeOutStart, curTime)));

	return flStrength;
}
