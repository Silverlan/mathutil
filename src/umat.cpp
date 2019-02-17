#include "mathutil/umat.h"
#include "mathutil/uvec.h"

using namespace umath;

Mat4 umat::identity() {return glm::mat4(
	1.f,0.f,0.f,0.f,
	0.f,1.f,0.f,0.f,
	0.f,0.f,1.f,0.f,
	0.f,0.f,0.f,1.f
);}
Mat4 umat::create(const Quat &q) {return glm::toMat4(q);}

Mat4 umat::create_from_axis_angle(const Vector3 &v,Float ang)
{
	Float c = cosf(ang);
	Float s = sinf(ang);
	Float t = 1.0f -c;
	Mat4 m(1);
	m[0][0] = c +v.x *v.x *t;
	m[1][1] = c +v.y *v.y *t;
	m[2][2] = c +v.z *v.z *t;
	Float tmp1 = v.x *v.y *t;
	Float tmp2 = v.z *s;
	m[1][0] = tmp1 +tmp2;
	m[0][1] = tmp1 -tmp2;
	tmp1 = v.x *v.z *t;
	tmp2 = v.y *s;
	m[2][0] = tmp1 -tmp2;
	m[0][2] = tmp1 +tmp2;
	tmp1 = v.y *v.z *t;
	tmp2 = v.x *s;
	m[2][1] = tmp1 +tmp2;
	m[1][2] = tmp1 -tmp2;
	return m;
}

Mat4 umat::look_at(const Vector3 &eye,const Vector3 &center,const Vector3 &up)
{
	Vector3 f = center -eye;
	uvec::normalize(&f);
	Vector3 u = uvec::get_normal(up);
	Vector3 s = glm::cross(f,u);
	if(s.x != 0 || s.y != 0 || s.z != 0)
		s = glm::normalize(s);
	u = glm::cross(s,f);

	Mat4 r(1);
	r[0][0] = s.x;
	r[1][0] = s.y;
	r[2][0] = s.z;
	r[0][1] = u.x;
	r[1][1] = u.y;
	r[2][1] = u.z;
	r[0][2] = -f.x;
	r[1][2] = -f.y;
	r[2][2] = -f.z;
	r[3][0] = -glm::dot(s,eye);
	r[3][1] = -glm::dot(u,eye);
	r[3][2] = glm::dot(f,eye);
	return r;
}

Mat3 umat::calc_covariance_matrix(const std::vector<Vector3> &points)
{
	auto avg = uvec::calc_average(points);
	return calc_covariance_matrix(points,avg);
}
Mat3 umat::calc_covariance_matrix(const std::vector<Vector3> &points,const Vector3 &avg)
{
	auto C = Mat3(0.f);
	for(auto p : points)
	{
		p -= avg;
		C += uvec::calc_outer_product(p,p);
	}
	return C;
}

Mat4 umat::create_from_axes(const Vector3 &forward,const Vector3 &right,const Vector3 &up)
{
	return Mat4(
		-right.x,-right.y,-right.z,0.f,
		up.x,up.y,up.z,0.f,
		forward.x,forward.y,forward.z,0.f,
		0.f,0.f,0.f,1.f
	);
}