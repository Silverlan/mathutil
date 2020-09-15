/*
* Copyright 2011-2014 Blender Foundation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#include "mathutil/umath_lighting.hpp"
#include <array>

// See cycles/src/kernel/svm/svm_math_util.h for original code

static constexpr std::array<std::array<float,3>,6> blackbody_table_r = {
	std::array<float,3>{2.52432244e+03f, -1.06185848e-03f, 3.11067539e+00f},
	std::array<float,3>{3.37763626e+03f, -4.34581697e-04f, 1.64843306e+00f},
	std::array<float,3>{4.10671449e+03f, -8.61949938e-05f, 6.41423749e-01f},
	std::array<float,3>{4.66849800e+03f, 2.85655028e-05f, 1.29075375e-01f},
	std::array<float,3>{4.60124770e+03f, 2.89727618e-05f, 1.48001316e-01f},
	std::array<float,3>{3.78765709e+03f, 9.36026367e-06f, 3.98995841e-01f}
};

static constexpr std::array<std::array<float,3>,6> blackbody_table_g = {
	std::array<float,3>{-7.50343014e+02f, 3.15679613e-04f, 4.73464526e-01f},
	std::array<float,3>{-1.00402363e+03f, 1.29189794e-04f, 9.08181524e-01f},
	std::array<float,3>{-1.22075471e+03f, 2.56245413e-05f, 1.20753416e+00f},
	std::array<float,3>{-1.42546105e+03f, -4.01730887e-05f, 1.44002695e+00f},
	std::array<float,3>{-1.18134453e+03f, -2.18913373e-05f, 1.30656109e+00f},
	std::array<float,3>{-5.00279505e+02f, -4.59745390e-06f, 1.09090465e+00f}
};

static constexpr std::array<std::array<float,4>,6> blackbody_table_b = {
	std::array<float,4>{0.0f, 0.0f, 0.0f, 0.0f}, /* zeros should be optimized by compiler */
	std::array<float,4>{0.0f, 0.0f, 0.0f, 0.0f},
	std::array<float,4>{0.0f, 0.0f, 0.0f, 0.0f},
	std::array<float,4>{-2.02524603e-11f, 1.79435860e-07f, -2.60561875e-04f, -1.41761141e-02f},
	std::array<float,4>{-2.22463426e-13f, -1.55078698e-08f, 3.81675160e-04f, -7.30646033e-01f},
	std::array<float,4>{6.72595954e-13f, -2.73059993e-08f, 4.24068546e-04f, -7.52204323e-01f}
};
Vector3 ulighting::color_temperature_to_color(Kelvin temperature)
{
	if (temperature >= 12'000.0f) {
		return Vector3{0.826270103f, 0.994478524f, 1.56626022f};
	}
	else if (temperature < 965.0f) {
		/* For 800 <= t < 965 color does not change in OSL implementation, so keep color the same */
		return Vector3{4.70366907f, 0.0f, 0.0f};
	}

	int i = (temperature >= 6'365.0f) ?
		5 :
		(temperature >= 3'315.0f) ? 4 :
		(temperature >= 1'902.0f) ? 3 : (temperature >= 1'449.0f) ? 2 : (temperature >= 1'167.0f) ? 1 : 0;

	const auto &r = blackbody_table_r.at(i);
	const auto &g = blackbody_table_g.at(i);
	const auto &b = blackbody_table_b.at(i);

	const float t_inv = 1.0f /temperature;
	return Vector3{r.at(0) * t_inv + r.at(1) * temperature + r.at(2),
		g.at(0) * t_inv + g.at(1) * temperature + g.at(2),
		((b.at(0) * temperature + b.at(1)) * temperature + b.at(2)) * temperature + b.at(3)};
}



// CIE colour matching functions xBar, yBar, and zBar for
//   wavelengths from 380 through 780 nanometers, every 5
//   nanometers.  For a wavelength lambda in this range:
//        cie_colour_match[(lambda - 380) / 5][0] = xBar
//        cie_colour_match[(lambda - 380) / 5][1] = yBar
//        cie_colour_match[(lambda - 380) / 5][2] = zBar
// See cycles/src/kernel/svm/svm_wavelength.h for original code
static constexpr std::array<std::array<float,3>,81> cie_colour_match = {
	std::array<float,3>{0.0014f, 0.0000f, 0.0065f}, std::array<float,3>{0.0022f, 0.0001f, 0.0105f}, std::array<float,3>{0.0042f, 0.0001f, 0.0201f},
	std::array<float,3>{0.0076f, 0.0002f, 0.0362f}, std::array<float,3>{0.0143f, 0.0004f, 0.0679f}, std::array<float,3>{0.0232f, 0.0006f, 0.1102f},
	std::array<float,3>{0.0435f, 0.0012f, 0.2074f}, std::array<float,3>{0.0776f, 0.0022f, 0.3713f}, std::array<float,3>{0.1344f, 0.0040f, 0.6456f},
	std::array<float,3>{0.2148f, 0.0073f, 1.0391f}, std::array<float,3>{0.2839f, 0.0116f, 1.3856f}, std::array<float,3>{0.3285f, 0.0168f, 1.6230f},
	std::array<float,3>{0.3483f, 0.0230f, 1.7471f}, std::array<float,3>{0.3481f, 0.0298f, 1.7826f}, std::array<float,3>{0.3362f, 0.0380f, 1.7721f},
	std::array<float,3>{0.3187f, 0.0480f, 1.7441f}, std::array<float,3>{0.2908f, 0.0600f, 1.6692f}, std::array<float,3>{0.2511f, 0.0739f, 1.5281f},
	std::array<float,3>{0.1954f, 0.0910f, 1.2876f}, std::array<float,3>{0.1421f, 0.1126f, 1.0419f}, std::array<float,3>{0.0956f, 0.1390f, 0.8130f},
	std::array<float,3>{0.0580f, 0.1693f, 0.6162f}, std::array<float,3>{0.0320f, 0.2080f, 0.4652f}, std::array<float,3>{0.0147f, 0.2586f, 0.3533f},
	std::array<float,3>{0.0049f, 0.3230f, 0.2720f}, std::array<float,3>{0.0024f, 0.4073f, 0.2123f}, std::array<float,3>{0.0093f, 0.5030f, 0.1582f},
	std::array<float,3>{0.0291f, 0.6082f, 0.1117f}, std::array<float,3>{0.0633f, 0.7100f, 0.0782f}, std::array<float,3>{0.1096f, 0.7932f, 0.0573f},
	std::array<float,3>{0.1655f, 0.8620f, 0.0422f}, std::array<float,3>{0.2257f, 0.9149f, 0.0298f}, std::array<float,3>{0.2904f, 0.9540f, 0.0203f},
	std::array<float,3>{0.3597f, 0.9803f, 0.0134f}, std::array<float,3>{0.4334f, 0.9950f, 0.0087f}, std::array<float,3>{0.5121f, 1.0000f, 0.0057f},
	std::array<float,3>{0.5945f, 0.9950f, 0.0039f}, std::array<float,3>{0.6784f, 0.9786f, 0.0027f}, std::array<float,3>{0.7621f, 0.9520f, 0.0021f},
	std::array<float,3>{0.8425f, 0.9154f, 0.0018f}, std::array<float,3>{0.9163f, 0.8700f, 0.0017f}, std::array<float,3>{0.9786f, 0.8163f, 0.0014f},
	std::array<float,3>{1.0263f, 0.7570f, 0.0011f}, std::array<float,3>{1.0567f, 0.6949f, 0.0010f}, std::array<float,3>{1.0622f, 0.6310f, 0.0008f},
	std::array<float,3>{1.0456f, 0.5668f, 0.0006f}, std::array<float,3>{1.0026f, 0.5030f, 0.0003f}, std::array<float,3>{0.9384f, 0.4412f, 0.0002f},
	std::array<float,3>{0.8544f, 0.3810f, 0.0002f}, std::array<float,3>{0.7514f, 0.3210f, 0.0001f}, std::array<float,3>{0.6424f, 0.2650f, 0.0000f},
	std::array<float,3>{0.5419f, 0.2170f, 0.0000f}, std::array<float,3>{0.4479f, 0.1750f, 0.0000f}, std::array<float,3>{0.3608f, 0.1382f, 0.0000f},
	std::array<float,3>{0.2835f, 0.1070f, 0.0000f}, std::array<float,3>{0.2187f, 0.0816f, 0.0000f}, std::array<float,3>{0.1649f, 0.0610f, 0.0000f},
	std::array<float,3>{0.1212f, 0.0446f, 0.0000f}, std::array<float,3>{0.0874f, 0.0320f, 0.0000f}, std::array<float,3>{0.0636f, 0.0232f, 0.0000f},
	std::array<float,3>{0.0468f, 0.0170f, 0.0000f}, std::array<float,3>{0.0329f, 0.0119f, 0.0000f}, std::array<float,3>{0.0227f, 0.0082f, 0.0000f},
	std::array<float,3>{0.0158f, 0.0057f, 0.0000f}, std::array<float,3>{0.0114f, 0.0041f, 0.0000f}, std::array<float,3>{0.0081f, 0.0029f, 0.0000f},
	std::array<float,3>{0.0058f, 0.0021f, 0.0000f}, std::array<float,3>{0.0041f, 0.0015f, 0.0000f}, std::array<float,3>{0.0029f, 0.0010f, 0.0000f},
	std::array<float,3>{0.0020f, 0.0007f, 0.0000f}, std::array<float,3>{0.0014f, 0.0005f, 0.0000f}, std::array<float,3>{0.0010f, 0.0004f, 0.0000f},
	std::array<float,3>{0.0007f, 0.0002f, 0.0000f}, std::array<float,3>{0.0005f, 0.0002f, 0.0000f}, std::array<float,3>{0.0003f, 0.0001f, 0.0000f},
	std::array<float,3>{0.0002f, 0.0001f, 0.0000f}, std::array<float,3>{0.0002f, 0.0001f, 0.0000f}, std::array<float,3>{0.0001f, 0.0000f, 0.0000f},
	std::array<float,3>{0.0001f, 0.0000f, 0.0000f}, std::array<float,3>{0.0001f, 0.0000f, 0.0000f}, std::array<float,3>{0.0000f, 0.0000f, 0.0000f}
};

static Vector3 xyz_to_rgb(const Vector3 &xyz)
{
	// See cycles/render/shader.cpp
	constexpr Vector3 xyz_to_r {3.2404542f, -1.5371385f, -0.4985314f};
	constexpr Vector3 xyz_to_g {-0.9692660f, 1.8760108f, 0.0415560f};
	constexpr Vector3 xyz_to_b {0.0556434f, -0.2040259f, 1.0572252f};
	constexpr Vector3 rgb_to_y {0.2126729f, 0.7151522f, 0.0721750f};
	return Vector3{uvec::dot(xyz_to_r,xyz),
		uvec::dot(xyz_to_g,xyz),
		uvec::dot(xyz_to_b,xyz)};
}
Vector3 ulighting::wavelength_to_color(Wavelength wavelength)
{
	float lambda_nm = wavelength;
	float ii = (lambda_nm - 380.0f) * (1.0f / 5.0f);  // scaled 0..80
	int i = static_cast<int>(ii);
	Vector3 color;

	if (i < 0 || i >= 80) {
		color = Vector3{0.0f, 0.0f, 0.0f};
	}
	else {
		ii -= i;
		const auto &c = cie_colour_match.at(i);
		color = uvec::lerp(Vector3{c.at(0), c.at(1), c.at(2)}, Vector3{c.at(3), c.at(4), c.at(5)}, ii);
	}

	color = xyz_to_rgb(color);
	color *= 1.0f / 2.52f;  // Empirical scale from lg to make all comps <= 1

							/* Clamp to zero if values are smaller */
	color = max(color, Vector3{0.0f, 0.0f, 0.0f});
	return color;
}

// Source: Blender Photographer Addon
static double srgb_to_luminance(const Vector3 &color)
{
	return 0.2126729 *color.r +0.7151522 *color.g +0.072175 *color.b;
}
Watt ulighting::cycles::lumen_to_watt_point(Lumen lumen,const Vector3 &color)
{
	return lumen *((1.f /ulighting::MAX_LIGHT_EFFICIENCY_EFFICACY) /srgb_to_luminance(color));
}
Watt ulighting::cycles::lumen_to_watt_spot(Lumen lumen,const Vector3 &color,umath::Degree spotSize)
{
	return lumen *((1.f /(ulighting::MAX_LIGHT_EFFICIENCY_EFFICACY *2.f *umath::pi *(1 -umath::cos(umath::deg_to_rad(spotSize) /2.f))) *4.f *umath::pi) /srgb_to_luminance(color));
}
Watt ulighting::cycles::lumen_to_watt_area(Lumen lumen,const Vector3 &color)
{
	return lumen *(1.f /(ulighting::MAX_LIGHT_EFFICIENCY_EFFICACY *2.f *(1.f -umath::cos(umath::deg_to_rad(155.f) /2.f))));
}
