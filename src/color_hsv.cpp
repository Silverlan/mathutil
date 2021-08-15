/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mathutil/color.h"

// Source: http://stackoverflow.com/a/6930407/2482983
void util::rgb_to_hsv(const Vector3 &rgb,double &hDeg,double &s,double &v)
{
	auto min = (rgb.x < rgb.y) ? rgb.x : rgb.y;
	min = (min < rgb.z) ? min  : rgb.z;

	auto max = (rgb.x > rgb.y) ? rgb.x : rgb.y;
	max = (max > rgb.z) ? max  : rgb.z;

	v = max;
	auto delta = max -min;
	if(delta < 0.00001)
	{
		s = 0;
		hDeg = 0; // undefined, maybe nan?
		return;
	}
	if(max > 0.0)
		s = delta /max;
	else
	{
		s = 0.0;
		hDeg = NAN;
		return;
	}
	if(rgb.x >= max)
		hDeg = (rgb.y -rgb.z) /delta;
	else
	{
		if(rgb.y >= max)
			hDeg = 2.0 +(rgb.z -rgb.x) /delta;
		else
			hDeg = 4.0 +(rgb.x -rgb.y) /delta;
	}
	hDeg *= 60.0;

	if(hDeg < 0.0)
		hDeg += 360.0;
}
void util::lerp_hsv(double &h0,double &s0,double &v0,double h1,double s1,double v1,double factor)
{
	auto d = h1 -h0;
	auto delta = (d +((abs(d) > 180.0) ? ((d < 0.0) ? 360.0 : -360.0) : 0.0));

	h0 = fmodf(((h0 +(delta *factor)) +360.0),360.0);
	s0 = umath::lerp(s0,s1,factor);
	v0 = umath::lerp(v0,v1,factor);
}
Vector3 util::hsv_to_rgb(double hDeg,double s,double v)
{
	Vector3 r;
	if(s <= 0.0)
	{
		r.x = v;
		r.y = v;
		r.z = v;
		return r;
	}
	auto hh = hDeg;
	if(hh >= 360.0)
		hh = 0.0;
	hh /= 60.0;
	auto i = static_cast<int32_t>(hh);
	auto ff = hh -i;
	auto p = v *(1.0 -s);
	auto q = v *(1.0 -(s *ff));
	auto t = v *(1.0 -(s *(1.0 -ff)));

	switch(i) {
		case 0:
			r.x = v;
			r.y = t;
			r.z = p;
			break;
		case 1:
			r.x = q;
			r.y = v;
			r.z = p;
			break;
		case 2:
			r.x = p;
			r.y = v;
			r.z = t;
			break;
		case 3:
			r.x = p;
			r.y = q;
			r.z = v;
			break;
		case 4:
			r.x = t;
			r.y = p;
			r.z = v;
			break;
		case 5:
		default:
			r.x = v;
			r.y = p;
			r.z = q;
			break;
	}
	return r;
}
