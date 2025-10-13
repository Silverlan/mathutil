// SPDX-FileCopyrightText: (c) 2020 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

module;

#include "mathutil/mathutildefinitions.h"

export module pragma.math:plane;

export import :euler_angles;

export namespace umath {
	class DLLMUTIL Plane {
	  public:
		Plane(const Vector3 &a, const Vector3 &b, const Vector3 &c);
		Plane(Vector3 n, const Vector3 &pos);
		Plane(Vector3 n, double d);
		Plane(const Vector4 &plane);
		Plane();
		Plane operator-() const;
	  private:
		Vector3 m_normal;
		Vector3 m_pos;
		Vector3 m_posCenter;
		double m_distance;
		void Initialize(Vector3 n, double d);
	  public:
		static bool GetPlaneIntersection(Vector3 *intersect, const Vector3 &na, const Vector3 &nb, const Vector3 &nc, double da, double db, double dc);
		const Vector3 &GetNormal() const;
		const Vector3 &GetPos() const;
		const Vector3 &GetCenterPos() const;
		Vector3 &GetNormal();
		Vector3 &GetPos();
		Vector3 &GetCenterPos();
		Vector4 ToVector4() const;
		double GetDistance() const;
		float GetDistance(const Vector3 &pos) const;
		void SetDistance(double d);
		void SetNormal(const Vector3 &n);
		void MoveToPos(const Vector3 &pos);
		void Rotate(const EulerAngles &ang);
	};
};
