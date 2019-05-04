/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mathutil/umath_equation_solver.h"
#include "mathutil/umath.h"

// Source: https://github.com/erich666/GraphicsGems/blob/240a34f2ad3fa577ef57be74920db6c4b00605e4/gems/Roots3And4.c
int32_t umath::solve_quadric(const std::array<double,3> &c,std::array<double,2> &s)
{
	/* normal form: x^2 + px + q = 0 */
	auto p = c[1] /(2 *c[2]);
	auto q = c[0] /c[2];

	auto D = p *p -q;

	if(is_zero(D))
	{
		s[0] = -p;
		return 1;
	}
	else if(D < 0)
		return 0;
	else /* if (D > 0) */
	{
		auto sqrt_D = sqrt(D);
		s[0] = sqrt_D -p;
		s[1] = -sqrt_D -p;
		return 2;
	}
}

int32_t umath::solve_cubic(const std::array<double,4> &c,std::array<double,3> &s)
{
	/* normal form: x^3 + Ax^2 + Bx + C = 0 */
	auto A = c[2] /c[3];
	auto B = c[1] /c[3];
	auto C = c[0] /c[3];

	/*  substitute x = y - A/3 to eliminate quadric term:
	x^3 +px + q = 0 */
	auto sq_A = A *A;
	auto p = 1.0 /3 *(-1.0 /3 *sq_A +B);
	auto q = 1.0 /2 *(2.0 /27 *A *sq_A -1.0 /3 *A *B +C);

	/* use Cardano's formula */
	auto cb_p = p *p *p;
	auto D = q *q +cb_p;

	int32_t num = 0;
	if(is_zero(D))
	{
		if(is_zero(q)) /* one triple solution */
		{
			s[0] = 0;
			num = 1;
		}
		else /* one single and one double solution */
		{
			auto u = cbrt(-q);
			s[0] = 2 *u;
			s[1] = -u;
			num = 2;
		}
	}
	else if(D < 0) /* Casus irreducibilis: three real solutions */
	{
		auto phi = 1.0 /3 *acos(-q /sqrt(-cb_p));
		auto t = 2 *sqrt(-p);

		s[0] = t *cos(phi);
		s[1] = -t *cos(phi +umath::pi /3);
		s[2] = -t *cos(phi -umath::pi /3);
		num = 3;
	}
	else /* one real solution */
	{
		auto sqrt_D = sqrt(D);
		auto u = cbrt(sqrt_D -q);
		auto v = - cbrt(sqrt_D +q);

		s[0] = u +v;
		num = 1;
	}
	/* resubstitute */
	auto sub = 1.0 /3 *A;

	for(auto i=decltype(num){0};i<num;++i)
		s[i] -= sub;

	return num;
}

int32_t umath::solve_quartic(const std::array<double,5> &c,std::array<double,4> &s)
{
	/* normal form: x^4 + Ax^3 + Bx^2 + Cx + D = 0 */
	auto A = c[3] /c[4];
	auto B = c[2] /c[4];
	auto C = c[1] /c[4];
	auto D = c[0] /c[4];

	/*  substitute x = y - A/4 to eliminate cubic term:
	x^4 + px^2 + qx + r = 0 */
	auto sq_A = A *A;
	auto p = -3.0 /8 *sq_A +B;
	auto q = 1.0 /8 *sq_A *A -1.0 /2 *A *B +C;
	auto r = -3.0 /256 *sq_A *sq_A +1.0 /16 *sq_A *B -1.0 /4 *A *C +D;

	std::array<double,4> coeffs;
	int32_t num = 0;
	if(is_zero(r))
	{
		/* no absolute term: y(y^3 + py + q) = 0 */
		coeffs[0] = q;
		coeffs[1] = p;
		coeffs[2] = 0;
		coeffs[3] = 1;

		num = solve_cubic(coeffs,*reinterpret_cast<std::array<double,3>*>(&s));

		s[num++] = 0;
	}
	else
	{
		/* solve the resolvent cubic ... */
		coeffs[0] = 1.0 /2 *r *p -1.0 /8 *q *q;
		coeffs[1] = -r;
		coeffs[2] = -1.0 /2 *p;
		coeffs[3] = 1;

		solve_cubic(coeffs,*reinterpret_cast<std::array<double,3>*>(&s));

		/* ... and take the one real solution ... */
		auto z = s[0];

		/* ... to build two quadric equations */
		auto u = z *z -r;
		auto v = 2 *z -p;

		if(is_zero(u))
			u = 0;
		else if(u > 0)
			u = sqrt(u);
		else
			return 0;

		if(is_zero(v))
			v = 0;
		else if(v > 0)
			v = sqrt(v);
		else
			return 0;

		coeffs[0] = z -u;
		coeffs[1] = (q < 0) ? -v : v;
		coeffs[2] = 1;

		num = solve_quadric(*reinterpret_cast<std::array<double,3>*>(&coeffs),*reinterpret_cast<std::array<double,2>*>(&s));

		coeffs[0] = z +u;
		coeffs[1] = (q < 0) ? v : -v;
		coeffs[2] = 1;

		num += solve_quadric(*reinterpret_cast<std::array<double,3>*>(&coeffs),*reinterpret_cast<std::array<double,2>*>(s.data() +num));
	}

	/* resubstitute */
	auto sub = 1.0 /4 *A;

	for(auto i=decltype(num){0};i<num;++i)
		s[i] -= sub;
	return num;
}
