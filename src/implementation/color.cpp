// SPDX-FileCopyrightText: (c) 2019 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

module;

#include "definitions.hpp"

module pragma.math;

import :camera;
import :core;
import pragma.string;

Color Color::CreateFromHexColor(const std::string &hexColor)
{
	auto r = pragma::string::substr(hexColor, 0, 2);
	auto g = pragma::string::substr(hexColor, 2, 2);
	auto b = pragma::string::substr(hexColor, 4, 2);
	auto a = pragma::string::substr(hexColor, 6, 2);
	return Color {static_cast<int16_t>(pragma::math::to_hex_number(r)), static_cast<int16_t>(pragma::math::to_hex_number(g)), static_cast<int16_t>(pragma::math::to_hex_number(b)), (a.empty() == false) ? static_cast<int16_t>(pragma::math::to_hex_number(a)) : static_cast<int16_t>(255)};
}
Color::Color(const std::string &str) : Color {0, 0, 0, 255} { pragma::string::string_to_array<Int16>(str, &r, pragma::string::cstring_to_number<int16_t>, 4); }
Color::Color(const Vector3 &v) : r(static_cast<Int16>(pragma::math::round(v.x * 255.f))), g(static_cast<Int16>(pragma::math::round(v.y * 255.f))), b(static_cast<Int16>(pragma::math::round(v.z * 255.f))), a(255) {}
Color::Color(const Vector4 &v) : r(static_cast<Int16>(pragma::math::round(v.x * 255.f))), g(static_cast<Int16>(pragma::math::round(v.y * 255.f))), b(static_cast<Int16>(pragma::math::round(v.z * 255.f))), a(static_cast<Int16>(pragma::math::round(v.w * 255.f))) {}
Color Color::GetComplementaryColor() const { return {static_cast<int16_t>(255 - r), static_cast<int16_t>(255 - g), static_cast<int16_t>(255 - b), static_cast<int16_t>(a)}; }
float Color::CalcPerceivedLuminance() const
{
	auto vCol = ToVector3();
	return 0.299 * vCol.r + 0.587 * vCol.g + 0.114 * vCol.b;
}
Color Color::GetContrastColor() const
{
	auto luminance = CalcPerceivedLuminance();
	return (luminance > 0.5f) ? colors::Black : colors::White;
}
std::string Color::ToString() const
{
	std::stringstream ss;
	ss << r << " " << g << " " << b << " " << a;
	return ss.str();
}
static std::string to_hex_string(int16_t v)
{
	auto sr = pragma::math::to_hex_string(pragma::math::clamp<int16_t>(v, 0, 255));
	if(sr.empty())
		sr = "00";
	else if(sr.size() == 1)
		sr = "0" + sr;
	else
		sr = sr.substr(0, 2);
	return sr;
};
std::string Color::ToHexColorRGB() const { return to_hex_string(r) + to_hex_string(g) + to_hex_string(b); }
std::string Color::ToHexColor() const { return to_hex_string(r) + to_hex_string(g) + to_hex_string(b) + to_hex_string(a); }
Color Color::Lerp(const Color &other, Float amount) const
{
	auto rgb0 = ToVector3();
	auto rgb1 = other.ToVector3();
	std::array<double, 3> hsv0;
	pragma::math::rgb_to_hsv(rgb0, hsv0[0], hsv0[1], hsv0[2]);

	std::array<double, 3> hsv1;
	pragma::math::rgb_to_hsv(rgb1, hsv1[0], hsv1[1], hsv1[2]);

	// Source: https://stackoverflow.com/a/33347522/2482983
	pragma::math::lerp_hsv(hsv0[0], hsv0[1], hsv0[2], hsv1[0], hsv1[1], hsv1[2], amount);

	Color r(pragma::math::hsv_to_rgb(hsv0[0], hsv0[1], hsv0[2]));
	r.a = pragma::math::lerp(this->a, other.a, amount);
	return r;
}
Color Color::LerpSimple(const Color &other, Float factor) const
{
	return Color {static_cast<Int16>(pragma::math::clamp(r + pragma::math::round((static_cast<Float>(other.r) - static_cast<Float>(r)) * factor), 0, 255)), static_cast<Int16>(pragma::math::clamp(g + pragma::math::round((static_cast<Float>(other.g) - static_cast<Float>(g)) * factor), 0, 255)),
	  static_cast<Int16>(pragma::math::clamp(b + pragma::math::round((static_cast<Float>(other.b) - static_cast<Float>(b)) * factor), 0, 255)), static_cast<Int16>(pragma::math::clamp(a + pragma::math::round((static_cast<Float>(other.a) - static_cast<Float>(a)) * factor), 0, 255))};
}

std::ostream &operator<<(std::ostream &os, const Color &col)
{
	os << col.r << " " << col.g << " " << col.b << " " << col.a;
	return os;
}
