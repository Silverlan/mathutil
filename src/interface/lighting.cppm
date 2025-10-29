// SPDX-FileCopyrightText: (c) 2019 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

module;

#include "mathutil/mathutildefinitions.h"

export module pragma.math:lighting;

import :core;
export import :types;

export {
	namespace ulighting {
		// See https://blendergrid.com/news/cycles-physically-correct-brightness
		constexpr float LUX_AT_WAVELENGTH_555NM_PER_IRRADIANCE = 683.f;

		enum class NaturalLightType : uint8_t {
			MatchFlame = 0,
			Candle,
			Flame,
			Sunset,
			Sunrise,
			HouseholdTungstenBulb,
			TungstenLamp500WTo1K,

			IncandescentLamp,
			WarmFluorescentLamp,
			LEDLamp,
			QuartzLight,

			StudioLamp,
			Floodlight,
			FluorescentLight,
			TungstenLamp2K,
			TungstenLamp5K,
			TungstenLamp10K,

			Moonlight,
			HorizonDaylight,
			TubularFluorescentLamp,

			VerticalDaylight,
			SunAtNoon,
			Daylight,
			SunThroughClouds,
			Overcast,
			RGBMonitorWhitePoint,
			OutdoorShade,
			PartlyCloudy,
			ClearBlueSky,

			Count
		};
		constexpr std::pair<Kelvin, Kelvin> get_color_temperature(NaturalLightType type)
		{
			// Source: https://blendergrid.com/news/cycles-physically-correct-brightness
			// and "Moving Frostbite to Physically Based Rendering 3.0"
			switch(type) {
			case NaturalLightType::MatchFlame:
				return {1'700, 1'800};
			case NaturalLightType::Candle:
				return {1'850, 1'930};
			case NaturalLightType::Sunset:
			case NaturalLightType::Sunrise:
				return {2'000, 3'000};
			case NaturalLightType::HouseholdTungstenBulb:
				return {2'500, 2'900};
			case NaturalLightType::TungstenLamp500WTo1K:
			case NaturalLightType::IncandescentLamp:
			case NaturalLightType::LEDLamp:
				return {3'000, 3'000};
			case NaturalLightType::QuartzLight:
			case NaturalLightType::StudioLamp:
			case NaturalLightType::Floodlight:
				return {3'200, 3'500};
			case NaturalLightType::FluorescentLight:
				return {3'200, 7'500};
			case NaturalLightType::TungstenLamp2K:
				return {3'275, 3'275};
			case NaturalLightType::TungstenLamp5K:
			case NaturalLightType::TungstenLamp10K:
				return {3'380, 3'380};
			case NaturalLightType::Moonlight:
				return {4'150, 4'150};
			case NaturalLightType::HorizonDaylight:
			case NaturalLightType::TubularFluorescentLamp:
				return {5'000, 5'000};
			case NaturalLightType::SunAtNoon:
			case NaturalLightType::VerticalDaylight:
				return {5'000, 5'400};
			case NaturalLightType::Daylight:
			case NaturalLightType::SunThroughClouds:
				return {5'500, 6'500};
			case NaturalLightType::Overcast:
				return {6'000, 7'500};
			case NaturalLightType::RGBMonitorWhitePoint:
				return {6'500, 6'500};
			case NaturalLightType::OutdoorShade:
				return {7'000, 8'000};
			case NaturalLightType::PartlyCloudy:
				return {8'000, 10'000};
			case NaturalLightType::WarmFluorescentLamp:
			case NaturalLightType::ClearBlueSky:
				return {15'000, 27'000};
			}
			static_assert(static_cast<std::underlying_type_t<NaturalLightType>>(NaturalLightType::Count) == 29);
			return {0, 0};
		}
		constexpr Kelvin get_average_color_temperature(NaturalLightType type)
		{
			auto range = get_color_temperature(type);
			return (range.first + range.second) / 2;
		}

		DLLMUTIL Vector3 color_temperature_to_color(Kelvin temperature);
		DLLMUTIL Vector3 wavelength_to_color(Wavelength wavelength);

		enum class LightSourceType : uint8_t {
			TungstenIncandescentLightBulb = 0,
			HalogenLamp,
			FluorescentLamp,
			LEDLamp,
			MetalHalideLamp,
			HighPressureSodiumVaporLamp,
			LowPressureSodiumVaporLamp,
			MercuryVaporLamp,
			D65StandardIlluminant,

			Count
		};
		constexpr LuminousEfficacy get_luminous_efficacy(LightSourceType type)
		{
			switch(type) {
			case LightSourceType::TungstenIncandescentLightBulb:
				return 15;
			case LightSourceType::HalogenLamp:
				return 20;
			case LightSourceType::FluorescentLamp:
				return 60;
			case LightSourceType::LEDLamp:
				return 90;
			case LightSourceType::MetalHalideLamp:
				return 87;
			case LightSourceType::HighPressureSodiumVaporLamp:
				return 117;
			case LightSourceType::LowPressureSodiumVaporLamp:
				return 150;
			case LightSourceType::MercuryVaporLamp:
				return 50;
			case LightSourceType::D65StandardIlluminant:
				return 177.83;
			}
			static_assert(static_cast<std::underlying_type_t<LightSourceType>>(LightSourceType::Count) == 9);
			return 0;
		}
		constexpr LuminousEfficacy MAX_LIGHT_EFFICIENCY_EFFICACY = 683; // Efficacy for 100% efficiency; Source: "Moving Frostbite to Physically Based Rendering 3.0"
		constexpr Watt lumens_to_watts(Lumen lumen, LuminousEfficacy luminousEfficacy = MAX_LIGHT_EFFICIENCY_EFFICACY) { return lumen / luminousEfficacy; }
		constexpr Watt lumens_to_watts(Lumen lumen, LightSourceType type) { return lumens_to_watts(lumen, get_luminous_efficacy(type)); }
		constexpr Lumen watts_to_lumens(Watt watt, LuminousEfficacy luminousEfficacy = MAX_LIGHT_EFFICIENCY_EFFICACY) { return watt * luminousEfficacy; }
		constexpr Lumen watts_to_lumens(Watt watt, LightSourceType type) { return watts_to_lumens(watt, get_luminous_efficacy(type)); }

		// Assumes a wave-length of 555nm
		constexpr Lux irradiance_to_lux(Irradiance irradiance) { return irradiance * LUX_AT_WAVELENGTH_555NM_PER_IRRADIANCE; }

		// Assumes a wave-length of 555nm
		constexpr Irradiance lux_to_irradiance(Lux lux) { return lux / LUX_AT_WAVELENGTH_555NM_PER_IRRADIANCE; }

		constexpr Lumen candela_to_lumens(Candela candela, umath::Radian apexAngleCos) { return candela * umath::calc_solid_angle(apexAngleCos); }

		constexpr Candela lumens_to_candela(Lumen lumens, umath::Radian apexAngleCos) { return lumens / umath::calc_solid_angle(apexAngleCos); }

		constexpr double srgb_to_luminance(const Vector3 &color) { return 0.2126729 * color.r + 0.7151522 * color.g + 0.072175 * color.b; }

		constexpr float calc_light_falloff(umath::Meter distance, umath::Meter radius)
		{
			float falloff = 0;
			float fDistOverRadius = umath::pow4(distance / radius);
			falloff = umath::pow2(umath::clamp(1.0 - fDistOverRadius, 0.0, 1.0));
			falloff /= umath::pow2(distance) + 1.0;
			return falloff;
		}

		constexpr float calc_light_falloff(umath::Meter distance) { return 1.f / umath::pow2(distance); }

		DLLMUTIL float calc_cone_falloff(const Vector3 &lightDir, const Vector3 &dirToLight, umath::Degree outerCutoffAngle, umath::Degree innerCutoffAngle);

		namespace cycles {
			DLLMUTIL Watt lumen_to_watt_point(Lumen lumen, const Vector3 &color);
			DLLMUTIL Watt lumen_to_watt_spot(Lumen lumen, const Vector3 &color, umath::Degree spotSize);
			DLLMUTIL Watt lumen_to_watt_area(Lumen lumen, const Vector3 &color);
		};
	};
}
