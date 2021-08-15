/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mathutil/color.h"
#include <sharedutils/util_string.h>
#include <sharedutils/util.h>
#include <sstream>

Color Color::CreateFromHexColor(const std::string &hexColor)
{
	auto r = ustring::substr(hexColor,0,2);
	auto g = ustring::substr(hexColor,2,2);
	auto b = ustring::substr(hexColor,4,2);
	auto a = ustring::substr(hexColor,6,2);
	return Color{
		static_cast<int16_t>(util::to_hex_number(r)),
		static_cast<int16_t>(util::to_hex_number(g)),
		static_cast<int16_t>(util::to_hex_number(b)),
		(a.empty() == false) ? static_cast<int16_t>(util::to_hex_number(a)) : 255
	};
}
Color::Color()
	: r(255),g(255),b(255),a(255)
{}
Color::Color(Int16 r,Int16 g,Int16 b,Int16 a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}
Color::Color(const std::string &str) : Color{0,0,0,255} {ustring::string_to_array<Int16,Int32>(str,&r,atoi,4);}
Color::Color(const Vector3 &v)
	: r(static_cast<Int16>(umath::round(v.x *255.f))),g(static_cast<Int16>(umath::round(v.y *255.f))),b(static_cast<Int16>(umath::round(v.z *255.f))),a(255)
{}
Color::Color(const Vector4 &v)
	: r(static_cast<Int16>(umath::round(v.x *255.f))),g(static_cast<Int16>(umath::round(v.y *255.f))),b(static_cast<Int16>(umath::round(v.z *255.f))),a(static_cast<Int16>(umath::round(v.w *255.f)))
{}
void Color::Set(const Color &col) {this->r = col.r; this->g = col.g; this->b = col.b; this->a = col.a;}
Vector3 Color::ToVector3() const {return Vector3{r /255.f,g /255.f,b /255.f};}
Vector4 Color::ToVector4() const {return Vector4{r /255.f,g /255.f,b /255.f,a /255.f};}
Color Color::GetComplementaryColor() const
{
	return {
		255 -r,
		255 -g,
		255 -b,
		a
	};
}
float Color::CalcPerceivedLuminance() const
{
	auto vCol = ToVector3();
	return 0.299 *vCol.r +0.587 *vCol.g +0.114 *vCol.b;
}
Color Color::GetContrastColor() const
{
	auto luminance = CalcPerceivedLuminance();
	return (luminance > 0.5f) ? Color::Black : Color::White;
}
std::string Color::ToString() const
{
	std::stringstream ss;
	ss<<r<<" "<<g<<" "<<b<<" "<<a;
	return ss.str();
}
static std::string to_hex_string(int16_t v)
{
	auto sr = util::to_hex_string(umath::clamp<int16_t>(v,0,255));
	if(sr.empty())
		sr = "00";
	else if(sr.size() == 1)
		sr = "0" +sr;
	else
		sr = sr.substr(0,2);
	return sr;
};
std::string Color::ToHexColorRGB() const
{
	return to_hex_string(r) +to_hex_string(g) +to_hex_string(b);
}
std::string Color::ToHexColor() const
{
	return to_hex_string(r) +to_hex_string(g) +to_hex_string(b) +to_hex_string(a);
}
Color Color::Lerp(const Color &other,Float amount) const
{
	auto rgb0 = ToVector3();
	auto rgb1 = other.ToVector3();
	std::array<double,3> hsv0;
	util::rgb_to_hsv(rgb0,hsv0[0],hsv0[1],hsv0[2]);

	std::array<double,3> hsv1;
	util::rgb_to_hsv(rgb1,hsv1[0],hsv1[1],hsv1[2]);

	// Source: https://stackoverflow.com/a/33347522/2482983
	util::lerp_hsv(hsv0[0],hsv0[1],hsv0[2],hsv1[0],hsv1[1],hsv1[2],amount);

	Color r(util::hsv_to_rgb(hsv0[0],hsv0[1],hsv0[2]));
	r.a = umath::lerp(this->a,other.a,amount);
	return r;
}
Color Color::LerpSimple(const Color &other,Float factor) const
{
	return Color{
		static_cast<Int16>(umath::clamp(r +umath::round((CFloat(other.r) -CFloat(r)) *factor),0,255)),
		static_cast<Int16>(umath::clamp(g +umath::round((CFloat(other.g) -CFloat(g)) *factor),0,255)),
		static_cast<Int16>(umath::clamp(b +umath::round((CFloat(other.b) -CFloat(b)) *factor),0,255)),
		static_cast<Int16>(umath::clamp(a +umath::round((CFloat(other.a) -CFloat(a)) *factor),0,255))
	};
}
Color operator/(const Color &col,Float f)
{
	return Color(
		CInt16(CFloat(col.r) /f),
		CInt16(CFloat(col.g) /f),
		CInt16(CFloat(col.b) /f),
		CInt16(CFloat(col.a) /f)
	);
}
Color operator*(const Color &col,Float f)
{
	return Color(
		CInt16(CFloat(col.r) *f),
		CInt16(CFloat(col.g) *f),
		CInt16(CFloat(col.b) *f),
		CInt16(CFloat(col.a) *f)
	);
}
Color operator+(const Color &a,const Color &b) {return Color(a.r +b.r,a.g +b.g,a.b +b.b,a.a +b.a);}
Color operator-(const Color &a,const Color &b) {return Color(a.r -b.r,a.g -b.g,a.b -b.b,a.a -b.a);}
Color operator-(const Color &a) {return Color(-a.r,-a.g,-a.b,-a.a);}
Color operator*(Float f,const Color &c) {return Color(static_cast<Int16>(f) *c.r,static_cast<Int16>(f) *c.g,static_cast<Int16>(f) *c.b,static_cast<Int16>(f) *c.a);}

std::ostream& operator<<(std::ostream &os,const Color &col)
{
	os<<col.r<<" "<<col.g<<" "<<col.b<<" "<<col.a;
	return os;
}
bool operator==(const Color &a,const Color &b) {return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;}
bool operator!=(const Color &a,const Color &b) {return (a == b) ? false : true;}

// Pink Colors
decltype(Color::Pink) Color::Pink = Color{255,192,203};
decltype(Color::LightPink) Color::LightPink = Color{255,182,193};
decltype(Color::HotPink) Color::HotPink = Color{255,105,180};
decltype(Color::DeepPink) Color::DeepPink = Color{255,20,147};
decltype(Color::PaleVioletRed) Color::PaleVioletRed = Color{219,112,147};
decltype(Color::MediumVioletRed) Color::MediumVioletRed = Color{199,21,133};

// Red Colors
decltype(Color::LightSalmon) Color::LightSalmon = Color{255,160,122};
decltype(Color::Salmon) Color::Salmon = Color{250,128,114};
decltype(Color::DarkSalmon) Color::DarkSalmon = Color{233,150,122};
decltype(Color::LightCoral) Color::LightCoral = Color{240,128,128};
decltype(Color::IndianRed) Color::IndianRed = Color{205,92,92};
decltype(Color::Crimson) Color::Crimson = Color{220,20,60};
decltype(Color::FireBrick) Color::FireBrick = Color{178,34,34};
decltype(Color::DarkRed) Color::DarkRed = Color{139,0,0};
decltype(Color::Red) Color::Red = Color{255,0,0};

// Orange Colors
decltype(Color::OrangeRed) Color::OrangeRed = Color{255,69,0};
decltype(Color::Tomato) Color::Tomato = Color{255,99,71};
decltype(Color::Coral) Color::Coral = Color{255,127,80};
decltype(Color::DarkOrange) Color::DarkOrange = Color{255,140,0};
decltype(Color::Orange) Color::Orange = Color{255,165,0};

// Yellow Colors
decltype(Color::Yellow) Color::Yellow = Color{255,255,0};
decltype(Color::LightYellow) Color::LightYellow = Color{255,255,224};
decltype(Color::LemonChiffon) Color::LemonChiffon = Color{255,250,205};
decltype(Color::LightGoldenrodYellow) Color::LightGoldenrodYellow = Color{250,250,210};
decltype(Color::PapayaWhip) Color::PapayaWhip = Color{255,239,213};
decltype(Color::Moccasin) Color::Moccasin = Color{255,228,181};
decltype(Color::PeachPuff) Color::PeachPuff = Color{255,218,185};
decltype(Color::PaleGoldenrod) Color::PaleGoldenrod = Color{238,232,170};
decltype(Color::Khaki) Color::Khaki = Color{240,230,140};
decltype(Color::DarkKhaki) Color::DarkKhaki = Color{189,183,107};
decltype(Color::Gold) Color::Gold = Color{255,215,0};

// Brown Colors
decltype(Color::Cornsilk) Color::Cornsilk = Color{255,248,220};
decltype(Color::BlanchedAlmond) Color::BlanchedAlmond = Color{255,235,205};
decltype(Color::Bisque) Color::Bisque = Color{255,228,196};
decltype(Color::NavajoWhite) Color::NavajoWhite = Color{255,222,173};
decltype(Color::Wheat) Color::Wheat = Color{245,222,179};
decltype(Color::BurlyWood) Color::BurlyWood = Color{222,184,135};
decltype(Color::Tan) Color::Tan = Color{210,180,140};
decltype(Color::RosyBrown) Color::RosyBrown = Color{188,143,143};
decltype(Color::SandyBrown) Color::SandyBrown = Color{244,164,96};
decltype(Color::Goldenrod) Color::Goldenrod = Color{218,165,32};
decltype(Color::DarkGoldenrod) Color::DarkGoldenrod = Color{184,134,11};
decltype(Color::Peru) Color::Peru = Color{205,133,63};
decltype(Color::Chocolate) Color::Chocolate = Color{210,105,30};
decltype(Color::SaddleBrown) Color::SaddleBrown = Color{139,69,19};
decltype(Color::Sienna) Color::Sienna = Color{160,82,45};
decltype(Color::Brown) Color::Brown = Color{165,42,42};
decltype(Color::Maroon) Color::Maroon = Color{128,0,0};

// Green Colors
decltype(Color::DarkOliveGreen) Color::DarkOliveGreen = Color{85,107,47};
decltype(Color::Olive) Color::Olive = Color{128,128,0};
decltype(Color::OliveDrab) Color::OliveDrab = Color{107,142,35};
decltype(Color::YellowGreen) Color::YellowGreen = Color{154,205,50};
decltype(Color::LimeGreen) Color::LimeGreen = Color{50,205,50};
decltype(Color::Lime) Color::Lime = Color{0,255,0};
decltype(Color::LawnGreen) Color::LawnGreen = Color{124,252,0};
decltype(Color::Chartreuse) Color::Chartreuse = Color{127,255,0};
decltype(Color::GreenYellow) Color::GreenYellow = Color{173,255,47};
decltype(Color::SpringGreen) Color::SpringGreen = Color{0,255,127};
decltype(Color::MediumSpringGreen) Color::MediumSpringGreen = Color{0,250,154};
decltype(Color::LightGreen) Color::LightGreen = Color{144,238,144};
decltype(Color::PaleGreen) Color::PaleGreen = Color{152,251,152};
decltype(Color::DarkSeaGreen) Color::DarkSeaGreen = Color{143,188,143};
decltype(Color::MediumAquamarine) Color::MediumAquamarine = Color{102,205,170};
decltype(Color::MediumSeaGreen) Color::MediumSeaGreen = Color{60,179,113};
decltype(Color::SeaGreen) Color::SeaGreen = Color{46,139,87};
decltype(Color::ForestGreen) Color::ForestGreen = Color{34,139,34};
decltype(Color::Green) Color::Green = Color{0,128,0};
decltype(Color::DarkGreen) Color::DarkGreen = Color{0,100,0};

// Cyan Colors
decltype(Color::Aqua) Color::Aqua = Color{0,255,255};
decltype(Color::Cyan) Color::Cyan = Color{0,255,255};
decltype(Color::LightCyan) Color::LightCyan = Color{224,255,255};
decltype(Color::PaleTurquoise) Color::PaleTurquoise = Color{175,238,238};
decltype(Color::Aquamarine) Color::Aquamarine = Color{127,255,212};
decltype(Color::Turquoise) Color::Turquoise = Color{64,224,208};
decltype(Color::MediumTurquoise) Color::MediumTurquoise = Color{72,209,204};
decltype(Color::DarkTurquoise) Color::DarkTurquoise = Color{0,206,209};
decltype(Color::LightSeaGreen) Color::LightSeaGreen = Color{32,178,170};
decltype(Color::CadetBlue) Color::CadetBlue = Color{95,158,160};
decltype(Color::DarkCyan) Color::DarkCyan = Color{0,139,139};
decltype(Color::Teal) Color::Teal = Color{0,128,128};

// Blue Colors
decltype(Color::LightSteelBlue) Color::LightSteelBlue = Color{176,196,222};
decltype(Color::PowderBlue) Color::PowderBlue = Color{176,224,230};
decltype(Color::LightBlue) Color::LightBlue = Color{173,216,230};
decltype(Color::SkyBlue) Color::SkyBlue = Color{135,206,235};
decltype(Color::LightSkyBlue) Color::LightSkyBlue = Color{135,206,250};
decltype(Color::DeepSkyBlue) Color::DeepSkyBlue = Color{0,191,255};
decltype(Color::DodgerBlue) Color::DodgerBlue = Color{30,144,255};
decltype(Color::CornflowerBlue) Color::CornflowerBlue = Color{100,149,237};
decltype(Color::SteelBlue) Color::SteelBlue = Color{70,130,180};
decltype(Color::RoyalBlue) Color::RoyalBlue = Color{65,105,225};
decltype(Color::Blue) Color::Blue = Color{0,0,255};
decltype(Color::MediumBlue) Color::MediumBlue = Color{0,0,205};
decltype(Color::DarkBlue) Color::DarkBlue = Color{0,0,139};
decltype(Color::Navy) Color::Navy = Color{0,0,128};
decltype(Color::MidnightBlue) Color::MidnightBlue = Color{25,25,112};

// Purple, Violet and Magenta Colors
decltype(Color::Lavender) Color::Lavender = Color{230,230,250};
decltype(Color::Thistle) Color::Thistle = Color{216,191,216};
decltype(Color::Plum) Color::Plum = Color{221,160,221};
decltype(Color::Violet) Color::Violet = Color{238,130,238};
decltype(Color::Orchid) Color::Orchid = Color{218,112,214};
decltype(Color::Fuchsia) Color::Fuchsia = Color{255,0,255};
decltype(Color::Magenta) Color::Magenta = Color{255,0,255};
decltype(Color::MediumOrchid) Color::MediumOrchid = Color{186,85,211};
decltype(Color::MediumPurple) Color::MediumPurple = Color{147,112,219};
decltype(Color::BlueViolet) Color::BlueViolet = Color{138,43,226};
decltype(Color::DarkViolet) Color::DarkViolet = Color{148,0,211};
decltype(Color::DarkOrchid) Color::DarkOrchid = Color{153,50,204};
decltype(Color::DarkMagenta) Color::DarkMagenta = Color{139,0,139};
decltype(Color::Purple) Color::Purple = Color{128,0,128};
decltype(Color::Indigo) Color::Indigo = Color{75,0,130};
decltype(Color::DarkSlateBlue) Color::DarkSlateBlue = Color{72,61,139};
decltype(Color::SlateBlue) Color::SlateBlue = Color{106,90,205};
decltype(Color::MediumSlateBlue) Color::MediumSlateBlue = Color{123,104,238};

// White Colors
decltype(Color::White) Color::White = Color{255,255,255};
decltype(Color::Snow) Color::Snow = Color{255,250,250};
decltype(Color::Honeydew) Color::Honeydew = Color{240,255,240};
decltype(Color::MintCream) Color::MintCream = Color{245,255,250};
decltype(Color::Azure) Color::Azure = Color{240,255,255};
decltype(Color::AliceBlue) Color::AliceBlue = Color{240,248,255};
decltype(Color::GhostWhite) Color::GhostWhite = Color{248,248,255};
decltype(Color::WhiteSmoke) Color::WhiteSmoke = Color{245,245,245};
decltype(Color::Seashell) Color::Seashell = Color{255,245,238};
decltype(Color::Beige) Color::Beige = Color{245,245,220};
decltype(Color::OldLace) Color::OldLace = Color{253,245,230};
decltype(Color::FloralWhite) Color::FloralWhite = Color{255,250,240};
decltype(Color::Ivory) Color::Ivory = Color{255,255,240};
decltype(Color::AntiqueWhite) Color::AntiqueWhite = Color{250,235,215};
decltype(Color::Linen) Color::Linen = Color{250,240,230};
decltype(Color::LavenderBlush) Color::LavenderBlush = Color{255,240,245};
decltype(Color::MistyRose) Color::MistyRose = Color{255,228,225};

// Grey and Black Colors
decltype(Color::Gainsboro) Color::Gainsboro = Color{220,220,220};
decltype(Color::LightGrey) Color::LightGrey = Color{211,211,211};
decltype(Color::Silver) Color::Silver = Color{192,192,192};
decltype(Color::DarkGray) Color::DarkGray = Color{169,169,169};
decltype(Color::Gray) Color::Gray = Color{128,128,128};
decltype(Color::DimGray) Color::DimGray = Color{105,105,105};
decltype(Color::LightSlateGray) Color::LightSlateGray = Color{119,136,153};
decltype(Color::SlateGray) Color::SlateGray = Color{112,128,144};
decltype(Color::DarkSlateGray) Color::DarkSlateGray = Color{47,79,79};
decltype(Color::Black) Color::Black = Color{0,0,0};
