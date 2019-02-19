#ifndef __UMATH_FLOAT_COMPRESSOR_H__
#define __UMATH_FLOAT_COMPRESSOR_H__

#include "mathutildefinitions.h"
#include <cinttypes>

// Source: Unknown
class DLLMUTIL FloatCompressor
{
	union Bits
	{
		float f;
		int32_t si;
		uint32_t ui;
	};
	bool hasNegatives;
	bool noLoss;
	int32_t _maxF;
	int32_t _minF;
	int32_t _epsF;
	int32_t _maxC;
	int32_t _zeroC;
	int32_t _pDelta;
	int32_t _nDelta;
	int _shift;

	static int32_t const signF = 0x80000000;
	static int32_t const absF = ~signF;
public:
	FloatCompressor(float min, float epsilon, float max, int precision);
	float clamp(float value);
	uint32_t compress(float value);
	float decompress(uint32_t value);
};

#endif
