#ifndef __EULERANGLES_H__
#define __EULERANGLES_H__
#include "mathutildefinitions.h"
#include "glmutil.h"
#include "umat.h"
#include <string>
#include <array>

class DLLMUTIL EulerAngles
{
public:
	EulerAngles();
	EulerAngles(Float p,Float y,Float r);
	EulerAngles(const EulerAngles &ang);
	EulerAngles(const Mat4 &mat);
	EulerAngles(const Quat &rot);
	EulerAngles(const Vector3 &forward);
	EulerAngles(const Vector3 &forward,const Vector3 &up);
	EulerAngles(const std::string &str);
	friend bool operator==(const EulerAngles &a,const EulerAngles &b);
	friend bool operator!=(const EulerAngles &a,const EulerAngles &b);
private:
	void Initialize(const Mat4 &mat);
public:
	Float p = 0.f;
	Float y = 0.f;
	Float r = 0.f;
	void Normalize(Float base);
	void Normalize();
	Vector3 Forward() const;
	Vector3 Right() const;
	Vector3 Up() const;
	Vector3 Up(Vector3 *forward,Vector3 *right) const;
	void GetOrientation(Vector3 *forward,Vector3 *right,Vector3 *up=nullptr) const;
	Mat4 ToMatrix() const;
	void Set(const EulerAngles &ang);
	EulerAngles Copy() const;
	void Approach(EulerAngles &ang,Float amount);
	static EulerAngles Approach(EulerAngles &a,EulerAngles &b,Float amount);
	Float &operator[](const uint8_t idx);
	const Float &operator[](const uint8_t idx) const;
	EulerAngles &operator=(const EulerAngles &other);
	void Flip();
};
DLLMUTIL EulerAngles operator/(const EulerAngles &ang,Float f);
DLLMUTIL EulerAngles operator*(const EulerAngles &ang,Float f);
DLLMUTIL EulerAngles operator/=(const EulerAngles &ang,Float f);
DLLMUTIL EulerAngles operator*=(const EulerAngles &ang,Float f);
DLLMUTIL EulerAngles operator*(Float f,const EulerAngles &a);

DLLMUTIL EulerAngles operator+(const EulerAngles &a,const EulerAngles &b);
DLLMUTIL EulerAngles operator-(const EulerAngles &a,const EulerAngles &b);
DLLMUTIL EulerAngles operator-(const EulerAngles &a);
DLLMUTIL EulerAngles operator+=(const EulerAngles &a,const EulerAngles &b);
DLLMUTIL EulerAngles operator-=(const EulerAngles &a,const EulerAngles &b);
DLLMUTIL std::ostream& operator<<(std::ostream &os,const EulerAngles &ang);

inline bool operator==(const EulerAngles &a,const EulerAngles &b) {return a.p == b.p && a.y == b.y && a.r == b.r;}
inline bool operator!=(const EulerAngles &a,const EulerAngles &b) {return (a == b) ? false : true;}

#endif