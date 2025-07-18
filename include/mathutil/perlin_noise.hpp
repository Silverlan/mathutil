// SPDX-FileCopyrightText: (c) 2021 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

#ifndef __UMATH_PERLIN_NOISE_HPP__
#define __UMATH_PERLIN_NOISE_HPP__

#include "mathutildefinitions.h"
#include <mathutil/glmutil.h>
#include <vector>

namespace umath {
	class DLLMUTIL PerlinNoise {
	  private:
		std::vector<int> m_permutation;
		double Fade(double t);
		double Lerp(double t, double a, double b);
		double Grad(int hash, double x, double y, double z);
	  public:
		PerlinNoise();
		PerlinNoise(unsigned int seed);
		double GetNoise(double x, double y, double z);
		double GetNoise(Vector3 &v);
	};
};

#endif
