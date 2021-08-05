/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/.
*
* Copyright (c) 2021 Silverlan
*/

#ifndef __UMATH_TRANSFORM_HPP__
#define __UMATH_TRANSFORM_HPP__

#include "mathutildefinitions.h"
#include "mathutil/umath.h"
#include "mathutil/glmutil.h"
#include "mathutil/uvec.h"
#include "mathutil/uquat.h"
#include <memory>
#include <sharedutils/def_handle.h>
#include <vector>

namespace umath
{
	class ScaledTransform;
	class DLLMUTIL Transform
	{
	public:
		constexpr Transform()
			: translation{},rotation{uquat::identity()}
		{}
		Transform(const Mat4 &t);
		Transform(const Vector3 &translation,const Quat &rotation);
		Transform(const Vector3 &translation);
		Transform(const Quat &rotation);
		~Transform()=default;

		bool operator==(const Transform &t) const;
		bool operator!=(const Transform &t) const {return !operator==(t);}

		const Vector3 &GetOrigin() const;
		const Quat &GetRotation() const;
		Vector3 &GetOrigin();
		Quat &GetRotation();
		void SetOrigin(const Vector3 &origin);
		void SetRotation(const Quat &rot);
		void SetIdentity();
		void TranslateGlobal(const Vector3 &v);
		void TranslateLocal(const Vector3 &v);
		void RotateGlobal(const Quat &rot);
		void RotateLocal(const Quat &rot);
		void Interpolate(const Transform &dst,float factor);
		void InterpolateToIdentity(float factor);
		Transform GetInverse() const;

		EulerAngles GetAngles() const;
		void SetAngles(const EulerAngles &ang);
		Vector3 GetForward() const;
		Vector3 GetRight() const;
		Vector3 GetUp() const;

		ScaledTransform operator*(const ScaledTransform &tOther) const;
		Transform operator*(const Transform &tOther) const;
		Transform &operator*=(const Transform &tOther);
		Vector3 operator*(const Vector3 &translation) const;
		Quat operator*(const Quat &rot) const;

		Transform operator*(float weight) const;
		Transform &operator*=(float weight);

		Mat4 ToMatrix() const;

		// Note: Getter/Setter methods should be preferred, these are public primarily to allow
		// the class to be used as a literal non-type template parameter
	public:
		Vector3 translation = {};
		Quat rotation = uquat::identity();
	};

	class DLLMUTIL ScaledTransform
		: public Transform
	{
	public:
		using Transform::Transform;
		ScaledTransform(const Transform &t,const Vector3 &scale);
		ScaledTransform(const Transform &t);
		ScaledTransform(const Vector3 &pos,const Quat &rot,const Vector3 &scale);
		void SetIdentity();
		const Vector3 &GetScale() const;
		Vector3 &GetScale();
		void SetScale(const Vector3 &scale);
		void Scale(const Vector3 &scale);
		void Interpolate(const ScaledTransform &dst,float factor);
		void InterpolateToIdentity(float factor);
		ScaledTransform GetInverse() const;
		ScaledTransform operator*(const ScaledTransform &tOther) const;
		ScaledTransform operator*(const Transform &tOther) const;
		ScaledTransform &operator*=(const ScaledTransform &tOther);
		ScaledTransform &operator*=(const Transform &tOther);
		Vector3 operator*(const Vector3 &translation) const;
		Quat operator*(const Quat &rot) const;

		bool operator==(const ScaledTransform &t) const;
		bool operator==(const Transform &t) const;
		bool operator!=(const ScaledTransform &t) const {return !operator==(t);}
		bool operator!=(const Transform &t) const {return !operator==(t);}

		ScaledTransform operator*(float weight) const;
		ScaledTransform &operator*=(float weight);

		Mat4 ToMatrix() const;

		// Note: Getter/Setter methods should be preferred, these are public primarily to allow
		// the class to be used as a literal non-type template parameter
	public:
		Vector3 scale = {1.f,1.f,1.f};
	};
};
DLLMUTIL Vector3 operator*(const Vector3 &v,const umath::Transform &t);
DLLMUTIL Vector3 &operator*=(Vector3 &v,const umath::Transform &t);
DLLMUTIL Quat operator*(const Quat &v,const umath::Transform &t);
DLLMUTIL Quat &operator*=(Quat &v,const umath::Transform &t);
DLLMUTIL umath::Transform operator*(float weight,const umath::Transform &t);

DLLMUTIL Vector3 operator*(const Vector3 &v,const umath::ScaledTransform &t);
DLLMUTIL Vector3 &operator*=(Vector3 &v,const umath::ScaledTransform &t);
DLLMUTIL Quat operator*(const Quat &v,const umath::ScaledTransform &t);
DLLMUTIL Quat &operator*=(Quat &v,const umath::ScaledTransform &t);
DLLMUTIL umath::ScaledTransform operator*(float weight,const umath::ScaledTransform &t);

#endif
