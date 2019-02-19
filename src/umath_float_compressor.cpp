#include "mathutil/umath_float_compressor.h"

// Source: Unknown
FloatCompressor::FloatCompressor(float min, float epsilon, float max, int precision)
{
	// legal values
	// min <= 0 < epsilon < max
	// 0 <= precision <= 23
	_shift = 23 - precision;
	Bits v;
	v.f = min;
	_minF = v.si;
	v.f = epsilon;
	_epsF = v.si;
	v.f = max;
	_maxF = v.si;
	hasNegatives = _minF < 0;
	noLoss = _shift == 0;
	int32_t pepsU, nepsU;
	if(noLoss) {
		nepsU = _epsF;
		pepsU = _epsF ^ signF;
		_maxC = _maxF ^ signF;
		_zeroC = signF;
	} else {
		nepsU = uint32_t(_epsF ^ signF) >> _shift;
		pepsU = uint32_t(_epsF) >> _shift;
		_maxC = uint32_t(_maxF) >> _shift;
		_zeroC = 0;
	}
	_pDelta = pepsU - _zeroC - 1;
	_nDelta = nepsU - _maxC - 1;
}

float FloatCompressor::clamp(float value)
{
	Bits v;
	v.f = value;
	int32_t max = _maxF;
	if(hasNegatives)
		max ^= (_minF ^ _maxF) & -(0 > v.si);
	v.si ^= (max ^ v.si) & -(v.si > max);
	v.si &= -(_epsF <= (v.si & absF));
	return v.f;
}

uint32_t FloatCompressor::compress(float value)
{
	Bits v;
	v.f = clamp(value);
	if(noLoss)
		v.si ^= signF;
	else
		v.ui >>= _shift;
	if(hasNegatives)
		v.si ^= ((v.si - _nDelta) ^ v.si) & -(v.si > _maxC);
	v.si ^= ((v.si - _pDelta) ^ v.si) & -(v.si > _zeroC);
	if(noLoss)
		v.si ^= signF;
	return v.ui;
}

float FloatCompressor::decompress(uint32_t value)
{
	Bits v;
	v.ui = value;
	if(noLoss)
		v.si ^= signF;
	v.si ^= ((v.si + _pDelta) ^ v.si) & -(v.si > _zeroC);
	if(hasNegatives)
		v.si ^= ((v.si + _nDelta) ^ v.si) & -(v.si > _maxC);
	if(noLoss)
		v.si ^= signF;
	else
		v.si <<= _shift;
	return v.f;
}
