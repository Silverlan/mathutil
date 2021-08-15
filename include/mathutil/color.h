/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __COLOR_H_
#define __COLOR_H_

#include "mathutildefinitions.h"
#include "umath.h"
#include "uvec.h"
#include <ostream>

struct DLLMUTIL Color
{
public:
	static Color CreateFromHexColor(const std::string &hexColor);
	Color();
	Color(Int16 r,Int16 g,Int16 b,Int16 a=255);
	Color(const std::string &str);
	Color(const Vector3 &v);
	Color(const Vector4 &v);
	Int16 r,g,b,a;
	void Set(const Color &col);
	Color Lerp(const Color &other,Float factor) const;
	// Fast but inaccurate RGB interpolation
	Color LerpSimple(const Color &other,Float factor) const;
	Vector3 ToVector3() const;
	Vector4 ToVector4() const;
	// Color will be clamped to range [0,255]
	std::string ToHexColor() const;
	std::string ToHexColorRGB() const;
	std::string ToString() const;

	float CalcPerceivedLuminance() const;
	Color GetComplementaryColor() const;
	Color GetContrastColor() const;

	// Pink Colors
	static const Color Pink;
	static const Color LightPink;
	static const Color HotPink;
	static const Color DeepPink;
	static const Color PaleVioletRed;
	static const Color MediumVioletRed;

	// Red Colors
	static const Color LightSalmon;
	static const Color Salmon;
	static const Color DarkSalmon;
	static const Color LightCoral;
	static const Color IndianRed;
	static const Color Crimson;
	static const Color FireBrick;
	static const Color DarkRed;
	static const Color Red;

	// Orange Colors
	static const Color OrangeRed;
	static const Color Tomato;
	static const Color Coral;
	static const Color DarkOrange;
	static const Color Orange;

	// Yellow Colors
	static const Color Yellow;
	static const Color LightYellow;
	static const Color LemonChiffon;
	static const Color LightGoldenrodYellow;
	static const Color PapayaWhip;
	static const Color Moccasin;
	static const Color PeachPuff;
	static const Color PaleGoldenrod;
	static const Color Khaki;
	static const Color DarkKhaki;
	static const Color Gold;

	// Brown Colors
	static const Color Cornsilk;
	static const Color BlanchedAlmond;
	static const Color Bisque;
	static const Color NavajoWhite;
	static const Color Wheat;
	static const Color BurlyWood;
	static const Color Tan;
	static const Color RosyBrown;
	static const Color SandyBrown;
	static const Color Goldenrod;
	static const Color DarkGoldenrod;
	static const Color Peru;
	static const Color Chocolate;
	static const Color SaddleBrown;
	static const Color Sienna;
	static const Color Brown;
	static const Color Maroon;

	// Green Colors
	static const Color DarkOliveGreen;
	static const Color Olive;
	static const Color OliveDrab;
	static const Color YellowGreen;
	static const Color LimeGreen;
	static const Color Lime;
	static const Color LawnGreen;
	static const Color Chartreuse;
	static const Color GreenYellow;
	static const Color SpringGreen;
	static const Color MediumSpringGreen;
	static const Color LightGreen;
	static const Color PaleGreen;
	static const Color DarkSeaGreen;
	static const Color MediumAquamarine;
	static const Color MediumSeaGreen;
	static const Color SeaGreen;
	static const Color ForestGreen;
	static const Color Green;
	static const Color DarkGreen;

	// Cyan Colors
	static const Color Aqua;
	static const Color Cyan;
	static const Color LightCyan;
	static const Color PaleTurquoise;
	static const Color Aquamarine;
	static const Color Turquoise;
	static const Color MediumTurquoise;
	static const Color DarkTurquoise;
	static const Color LightSeaGreen;
	static const Color CadetBlue;
	static const Color DarkCyan;
	static const Color Teal;

	// Blue Colors
	static const Color LightSteelBlue;
	static const Color PowderBlue;
	static const Color LightBlue;
	static const Color SkyBlue;
	static const Color LightSkyBlue;
	static const Color DeepSkyBlue;
	static const Color DodgerBlue;
	static const Color CornflowerBlue;
	static const Color SteelBlue;
	static const Color RoyalBlue;
	static const Color Blue;
	static const Color MediumBlue;
	static const Color DarkBlue;
	static const Color Navy;
	static const Color MidnightBlue;

	// Purple, Violet and Magenta Colors
	static const Color Lavender;
	static const Color Thistle;
	static const Color Plum;
	static const Color Violet;
	static const Color Orchid;
	static const Color Fuchsia;
	static const Color Magenta;
	static const Color MediumOrchid;
	static const Color MediumPurple;
	static const Color BlueViolet;
	static const Color DarkViolet;
	static const Color DarkOrchid;
	static const Color DarkMagenta;
	static const Color Purple;
	static const Color Indigo;
	static const Color DarkSlateBlue;
	static const Color SlateBlue;
	static const Color MediumSlateBlue;

	// White Colors
	static const Color White;
	static const Color Snow;
	static const Color Honeydew;
	static const Color MintCream;
	static const Color Azure;
	static const Color AliceBlue;
	static const Color GhostWhite;
	static const Color WhiteSmoke;
	static const Color Seashell;
	static const Color Beige;
	static const Color OldLace;
	static const Color FloralWhite;
	static const Color Ivory;
	static const Color AntiqueWhite;
	static const Color Linen;
	static const Color LavenderBlush;
	static const Color MistyRose;

	// Grey and Black Colors
	static const Color Gainsboro;
	static const Color LightGrey;
	static const Color Silver;
	static const Color DarkGray;
	static const Color Gray;
	static const Color DimGray;
	static const Color LightSlateGray;
	static const Color SlateGray;
	static const Color DarkSlateGray;
	static const Color Black;
};

namespace util
{
	DLLMUTIL void rgb_to_hsv(const Vector3 &rgb,double &hDeg,double &s,double &v);
	DLLMUTIL Vector3 hsv_to_rgb(double hDeg,double s,double v);
	DLLMUTIL void lerp_hsv(double &h0,double &s0,double &v0,double h1,double s1,double v1,double factor);
};

DLLMUTIL Color operator/(const Color &col,Float f);
DLLMUTIL Color operator*(const Color &col,Float f);
DLLMUTIL Color operator+(const Color &a,const Color &b);
DLLMUTIL Color operator-(const Color &a,const Color &b);
DLLMUTIL Color operator-(const Color &a);
DLLMUTIL Color operator*(Float f,const Color &c);

DLLMUTIL std::ostream& operator<<(std::ostream &os,const Color &col);
DLLMUTIL bool operator==(const Color &a,const Color &b);
DLLMUTIL bool operator!=(const Color &a,const Color &b);

#endif