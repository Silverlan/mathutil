// SPDX-FileCopyrightText: (c) 2019 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

module;

#include "mathutil/mathutildefinitions.h"

export module pragma.math:random;

export import std.compat;

export {
	namespace umath {
		DLLMUTIL std::mt19937 &get_default_random_generator();
		template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
		T random_int(T min, T max);
		template<typename T, typename = std::enable_if_t<std::is_floating_point<T>::value>>
		T random_real(T min, T max);
	};

	template<typename T, typename>
	T umath::random_int(T min, T max)
	{
		if(max < min)
			return random_int(max, min);
		std::uniform_int_distribution<T> dis(min, max);
		return dis(get_default_random_generator());
	}

	template<typename T, typename>
	T umath::random_real(T min, T max)
	{
		if(max < min)
			return random_real(max, min);
		std::uniform_real_distribution<T> dis(min, max);
		return dis(get_default_random_generator());
	}
}
