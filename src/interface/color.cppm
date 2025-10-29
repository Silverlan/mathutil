// SPDX-FileCopyrightText: (c) 2019 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

module;

#include "mathutil/mathutildefinitions.h"

export module pragma.math:color;

export import :types;

export {
	struct DLLMUTIL Color {
	public:
		static Color CreateFromHexColor(const std::string &hexColor);
		constexpr Color() : r(255), g(255), b(255), a(255) {}
		constexpr Color(Int16 r, Int16 g, Int16 b, Int16 a = 255)
		{
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;
		}

		Color(const std::string &str);
		Color(const Vector3 &v);
		Color(const Vector4 &v);
		Int16 r, g, b, a;
		constexpr void Set(const Color &col)
		{
			this->r = col.r;
			this->g = col.g;
			this->b = col.b;
			this->a = col.a;
		}
		Color Lerp(const Color &other, Float factor) const;
		// Fast but inaccurate RGB interpolation
		Color LerpSimple(const Color &other, Float factor) const;
		constexpr Vector3 ToVector3() const { return Vector3 {r / 255.f, g / 255.f, b / 255.f}; }
		constexpr Vector4 ToVector4() const { return Vector4 {r / 255.f, g / 255.f, b / 255.f, a / 255.f}; }
		// Color will be clamped to range [0,255]
		std::string ToHexColor() const;
		std::string ToHexColorRGB() const;
		std::string ToString() const;

		float CalcPerceivedLuminance() const;
		Color GetComplementaryColor() const;
		Color GetContrastColor() const;
	};

	namespace util {
		DLLMUTIL void rgb_to_hsv(const Vector3 &rgb, double &hDeg, double &s, double &v);
		DLLMUTIL Vector3 hsv_to_rgb(double hDeg, double s, double v);
		DLLMUTIL void lerp_hsv(double &h0, double &s0, double &v0, double h1, double s1, double v1, double factor);
	};

	constexpr Color operator/(const Color &col, Float f) { return Color(static_cast<Int16>(static_cast<Float>(col.r) / f), static_cast<Int16>(static_cast<Float>(col.g) / f), static_cast<Int16>(static_cast<Float>(col.b) / f), static_cast<Int16>(static_cast<Float>(col.a) / f)); }
	constexpr Color operator*(const Color &col, Float f) { return Color(static_cast<Int16>(static_cast<Float>(col.r) * f), static_cast<Int16>(static_cast<Float>(col.g) * f), static_cast<Int16>(static_cast<Float>(col.b) * f), static_cast<Int16>(static_cast<Float>(col.a) * f)); }
	constexpr Color operator+(const Color &a, const Color &b) { return Color(a.r + b.r, a.g + b.g, a.b + b.b, a.a + b.a); }
	constexpr Color operator-(const Color &a, const Color &b) { return Color(a.r - b.r, a.g - b.g, a.b - b.b, a.a - b.a); }
	constexpr Color operator-(const Color &a) { return Color(-a.r, -a.g, -a.b, -a.a); }
	constexpr Color operator*(Float f, const Color &c) { return Color(static_cast<Int16>(f) * c.r, static_cast<Int16>(f) * c.g, static_cast<Int16>(f) * c.b, static_cast<Int16>(f) * c.a); }

	DLLMUTIL std::ostream &operator<<(std::ostream &os, const Color &col);
	constexpr bool operator==(const Color &a, const Color &b) { return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a; }
	constexpr bool operator!=(const Color &a, const Color &b) { return (a == b) ? false : true; }

	namespace colors {
		// Pink Colors
		inline constexpr Color Pink {255, 192, 203};
		inline constexpr Color LightPink {255, 182, 193};
		inline constexpr Color HotPink {255, 105, 180};
		inline constexpr Color DeepPink {255, 20, 147};
		inline constexpr Color PaleVioletRed {219, 112, 147};
		inline constexpr Color MediumVioletRed {199, 21, 133};

		// Red Colors
		inline constexpr Color LightSalmon {255, 160, 122};
		inline constexpr Color Salmon {250, 128, 114};
		inline constexpr Color DarkSalmon {233, 150, 122};
		inline constexpr Color LightCoral {240, 128, 128};
		inline constexpr Color IndianRed {205, 92, 92};
		inline constexpr Color Crimson {220, 20, 60};
		inline constexpr Color FireBrick {178, 34, 34};
		inline constexpr Color DarkRed {139, 0, 0};
		inline constexpr Color Red {255, 0, 0};

		// Orange Colors
		inline constexpr Color OrangeRed {255, 69, 0};
		inline constexpr Color Tomato {255, 99, 71};
		inline constexpr Color Coral {255, 127, 80};
		inline constexpr Color DarkOrange {255, 140, 0};
		inline constexpr Color Orange {255, 165, 0};

		// Yellow Colors
		inline constexpr Color Yellow {255, 255, 0};
		inline constexpr Color LightYellow {255, 255, 224};
		inline constexpr Color LemonChiffon {255, 250, 205};
		inline constexpr Color LightGoldenrodYellow {250, 250, 210};
		inline constexpr Color PapayaWhip {255, 239, 213};
		inline constexpr Color Moccasin {255, 228, 181};
		inline constexpr Color PeachPuff {255, 218, 185};
		inline constexpr Color PaleGoldenrod {238, 232, 170};
		inline constexpr Color Khaki {240, 230, 140};
		inline constexpr Color DarkKhaki {189, 183, 107};
		inline constexpr Color Gold {255, 215, 0};

		// Brown Colors
		inline constexpr Color Cornsilk {255, 248, 220};
		inline constexpr Color BlanchedAlmond {255, 235, 205};
		inline constexpr Color Bisque {255, 228, 196};
		inline constexpr Color NavajoWhite {255, 222, 173};
		inline constexpr Color Wheat {245, 222, 179};
		inline constexpr Color BurlyWood {222, 184, 135};
		inline constexpr Color Tan {210, 180, 140};
		inline constexpr Color RosyBrown {188, 143, 143};
		inline constexpr Color SandyBrown {244, 164, 96};
		inline constexpr Color Goldenrod {218, 165, 32};
		inline constexpr Color DarkGoldenrod {184, 134, 11};
		inline constexpr Color Peru {205, 133, 63};
		inline constexpr Color Chocolate {210, 105, 30};
		inline constexpr Color SaddleBrown {139, 69, 19};
		inline constexpr Color Sienna {160, 82, 45};
		inline constexpr Color Brown {165, 42, 42};
		inline constexpr Color Maroon {128, 0, 0};

		// Green Colors
		inline constexpr Color DarkOliveGreen {85, 107, 47};
		inline constexpr Color Olive {128, 128, 0};
		inline constexpr Color OliveDrab {107, 142, 35};
		inline constexpr Color YellowGreen {154, 205, 50};
		inline constexpr Color LimeGreen {50, 205, 50};
		inline constexpr Color Lime {0, 255, 0};
		inline constexpr Color LawnGreen {124, 252, 0};
		inline constexpr Color Chartreuse {127, 255, 0};
		inline constexpr Color GreenYellow {173, 255, 47};
		inline constexpr Color SpringGreen {0, 255, 127};
		inline constexpr Color MediumSpringGreen {0, 250, 154};
		inline constexpr Color LightGreen {144, 238, 144};
		inline constexpr Color PaleGreen {152, 251, 152};
		inline constexpr Color DarkSeaGreen {143, 188, 143};
		inline constexpr Color MediumAquamarine {102, 205, 170};
		inline constexpr Color MediumSeaGreen {60, 179, 113};
		inline constexpr Color SeaGreen {46, 139, 87};
		inline constexpr Color ForestGreen {34, 139, 34};
		inline constexpr Color Green {0, 128, 0};
		inline constexpr Color DarkGreen {0, 100, 0};

		// Cyan Colors
		inline constexpr Color Aqua {0, 255, 255};
		inline constexpr Color Cyan {0, 255, 255};
		inline constexpr Color LightCyan {224, 255, 255};
		inline constexpr Color PaleTurquoise {175, 238, 238};
		inline constexpr Color Aquamarine {127, 255, 212};
		inline constexpr Color Turquoise {64, 224, 208};
		inline constexpr Color MediumTurquoise {72, 209, 204};
		inline constexpr Color DarkTurquoise {0, 206, 209};
		inline constexpr Color LightSeaGreen {32, 178, 170};
		inline constexpr Color CadetBlue {95, 158, 160};
		inline constexpr Color DarkCyan {0, 139, 139};
		inline constexpr Color Teal {0, 128, 128};

		// Blue Colors
		inline constexpr Color LightSteelBlue {176, 196, 222};
		inline constexpr Color PowderBlue {176, 224, 230};
		inline constexpr Color LightBlue {173, 216, 230};
		inline constexpr Color SkyBlue {135, 206, 235};
		inline constexpr Color LightSkyBlue {135, 206, 250};
		inline constexpr Color DeepSkyBlue {0, 191, 255};
		inline constexpr Color DodgerBlue {30, 144, 255};
		inline constexpr Color CornflowerBlue {100, 149, 237};
		inline constexpr Color SteelBlue {70, 130, 180};
		inline constexpr Color RoyalBlue {65, 105, 225};
		inline constexpr Color Blue {0, 0, 255};
		inline constexpr Color MediumBlue {0, 0, 205};
		inline constexpr Color DarkBlue {0, 0, 139};
		inline constexpr Color Navy {0, 0, 128};
		inline constexpr Color MidnightBlue {25, 25, 112};

		// Purple, Violet and Magenta Colors
		inline constexpr Color Lavender {230, 230, 250};
		inline constexpr Color Thistle {216, 191, 216};
		inline constexpr Color Plum {221, 160, 221};
		inline constexpr Color Violet {238, 130, 238};
		inline constexpr Color Orchid {218, 112, 214};
		inline constexpr Color Fuchsia {255, 0, 255};
		inline constexpr Color Magenta {255, 0, 255};
		inline constexpr Color MediumOrchid {186, 85, 211};
		inline constexpr Color MediumPurple {147, 112, 219};
		inline constexpr Color BlueViolet {138, 43, 226};
		inline constexpr Color DarkViolet {148, 0, 211};
		inline constexpr Color DarkOrchid {153, 50, 204};
		inline constexpr Color DarkMagenta {139, 0, 139};
		inline constexpr Color Purple {128, 0, 128};
		inline constexpr Color Indigo {75, 0, 130};
		inline constexpr Color DarkSlateBlue {72, 61, 139};
		inline constexpr Color SlateBlue {106, 90, 205};
		inline constexpr Color MediumSlateBlue {123, 104, 238};

		// White Colors
		inline constexpr Color White {255, 255, 255};
		inline constexpr Color Snow {255, 250, 250};
		inline constexpr Color Honeydew {240, 255, 240};
		inline constexpr Color MintCream {245, 255, 250};
		inline constexpr Color Azure {240, 255, 255};
		inline constexpr Color AliceBlue {240, 248, 255};
		inline constexpr Color GhostWhite {248, 248, 255};
		inline constexpr Color WhiteSmoke {245, 245, 245};
		inline constexpr Color Seashell {255, 245, 238};
		inline constexpr Color Beige {245, 245, 220};
		inline constexpr Color OldLace {253, 245, 230};
		inline constexpr Color FloralWhite {255, 250, 240};
		inline constexpr Color Ivory {255, 255, 240};
		inline constexpr Color AntiqueWhite {250, 235, 215};
		inline constexpr Color Linen {250, 240, 230};
		inline constexpr Color LavenderBlush {255, 240, 245};
		inline constexpr Color MistyRose {255, 228, 225};

		// Grey and Black Colors
		inline constexpr Color Gainsboro {220, 220, 220};
		inline constexpr Color LightGrey {211, 211, 211};
		inline constexpr Color Silver {192, 192, 192};
		inline constexpr Color DarkGray {169, 169, 169};
		inline constexpr Color Gray {128, 128, 128};
		inline constexpr Color DimGray {105, 105, 105};
		inline constexpr Color LightSlateGray {119, 136, 153};
		inline constexpr Color SlateGray {112, 128, 144};
		inline constexpr Color DarkSlateGray {47, 79, 79};
		inline constexpr Color Black {0, 0, 0};
	}
}
