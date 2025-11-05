// SPDX-FileCopyrightText: (c) 2019 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

module;

#include "definitions.hpp"

export module pragma.math:euler_angles;

export import :types;

export {
	class DLLMUTIL EulerAngles {
	public:
		constexpr EulerAngles() : EulerAngles {0.f, 0.f, 0.f} {}
		constexpr EulerAngles(Float p, Float y, Float r) : p {p}, y {y}, r {r} {}
		constexpr EulerAngles(const EulerAngles &ang) : EulerAngles {ang.p, ang.y, ang.r} {}
		EulerAngles(const Mat4 &mat);
		EulerAngles(const Quat &rot);
		explicit EulerAngles(const Vector3 &forward);
		EulerAngles(const Vector3 &forward, const Vector3 &up);
		EulerAngles(const std::string &str);

		friend bool operator==(const EulerAngles &a, const EulerAngles &b);
		friend bool operator!=(const EulerAngles &a, const EulerAngles &b);
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
		Vector3 Up(Vector3 *forward, Vector3 *right) const;
		void GetOrientation(Vector3 *forward, Vector3 *right, Vector3 *up = nullptr) const;
		Mat4 ToMatrix() const;
		void Set(const EulerAngles &ang);
		EulerAngles Copy() const;
		void Approach(EulerAngles &ang, Float amount);
		static EulerAngles Approach(EulerAngles &a, EulerAngles &b, Float amount);
		Float &operator[](const uint8_t idx);
		const Float &operator[](const uint8_t idx) const;
		EulerAngles &operator=(const EulerAngles &other);
		void Flip();
	};
	DLLMUTIL EulerAngles operator/(const EulerAngles &ang, Float f);
	DLLMUTIL EulerAngles operator*(const EulerAngles &ang, Float f);
	DLLMUTIL EulerAngles operator/=(const EulerAngles &ang, Float f);
	DLLMUTIL EulerAngles operator*=(const EulerAngles &ang, Float f);
	DLLMUTIL EulerAngles operator*(Float f, const EulerAngles &a);

	DLLMUTIL EulerAngles operator+(const EulerAngles &a, const EulerAngles &b);
	DLLMUTIL EulerAngles operator-(const EulerAngles &a, const EulerAngles &b);
	DLLMUTIL EulerAngles operator-(const EulerAngles &a);
	DLLMUTIL EulerAngles operator+=(const EulerAngles &a, const EulerAngles &b);
	DLLMUTIL EulerAngles operator-=(const EulerAngles &a, const EulerAngles &b);
	DLLMUTIL std::ostream &operator<<(std::ostream &os, const EulerAngles &ang);

	inline bool operator==(const EulerAngles &a, const EulerAngles &b) { return a.p == b.p && a.y == b.y && a.r == b.r; }
	inline bool operator!=(const EulerAngles &a, const EulerAngles &b) { return (a == b) ? false : true; }
}
