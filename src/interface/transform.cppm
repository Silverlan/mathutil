// SPDX-FileCopyrightText: (c) 2020 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

module;

#include "mathutil/mathutildefinitions.h"
#include "mathutil/glmutil.h"
#include <memory>
#include <vector>

export module pragma.math:transform;

export import :plane;
import :quaternion;
export import :types;
import :vector;

export {
	namespace umath {
		enum class CoordinateSpace : uint32_t {
			Local = 0,
			World,
			Object,
			View,
			Screen,

			Count
		};

		class ScaledTransform;
		class DLLMUTIL Transform {
		public:
			constexpr Transform() : translation {}, rotation {uquat::identity()} {}
			Transform(const Mat4 &t);
			Transform(const Vector3 &translation, const Quat &rotation);
			Transform(const Vector3 &translation);
			Transform(const Quat &rotation);
			~Transform() = default;

			bool operator==(const Transform &t) const;
			bool operator!=(const Transform &t) const { return !operator==(t); }

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
			void Interpolate(const Transform &dst, float factor);
			void InterpolateToIdentity(float factor);
			Transform GetInverse() const;

			Plane ToPlane() const;

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
			Plane operator*(const Plane &plane) const;

			Transform operator*(float weight) const;
			Transform &operator*=(float weight);

			Mat4 ToMatrix() const;
			void Reflect(const Vector3 &n, float d);

			// Note: Getter/Setter methods should be preferred, these are public primarily to allow
			// the class to be used as a literal non-type template parameter
		public:
			Vector3 translation = {};
			Quat rotation = uquat::identity();
		};

		class DLLMUTIL ScaledTransform : public Transform {
		public:
			constexpr ScaledTransform() : Transform {}, scale {uvec::IDENTITY_SCALE} {}
			ScaledTransform(const Mat4 &t);
			ScaledTransform(const Vector3 &translation, const Quat &rotation) : Transform {translation, rotation} {}
			ScaledTransform(const Vector3 &translation) : Transform {translation} {}
			ScaledTransform(const Quat &rotation) : Transform {rotation} {}
			ScaledTransform(const Transform &t, const Vector3 &scale);
			ScaledTransform(const Transform &t);
			ScaledTransform(const Vector3 &pos, const Quat &rot, const Vector3 &scale);
			void SetIdentity();
			const Vector3 &GetScale() const;
			Vector3 &GetScale();
			void SetScale(const Vector3 &scale);
			void Scale(const Vector3 &scale);
			void Interpolate(const ScaledTransform &dst, float factor);
			void InterpolateToIdentity(float factor);
			ScaledTransform GetInverse() const;
			ScaledTransform operator*(const ScaledTransform &tOther) const;
			ScaledTransform operator*(const Transform &tOther) const;
			ScaledTransform &operator*=(const ScaledTransform &tOther);
			ScaledTransform &operator*=(const Transform &tOther);
			Vector3 operator*(const Vector3 &translation) const;
			Quat operator*(const Quat &rot) const;
			Plane operator*(const Plane &plane) const;

			bool operator==(const ScaledTransform &t) const;
			bool operator==(const Transform &t) const;
			bool operator!=(const ScaledTransform &t) const { return !operator==(t); }
			bool operator!=(const Transform &t) const { return !operator==(t); }

			ScaledTransform operator*(float weight) const;
			ScaledTransform &operator*=(float weight);

			Mat4 ToMatrix() const;

			// Note: Getter/Setter methods should be preferred, these are public primarily to allow
			// the class to be used as a literal non-type template parameter
		public:
			Vector3 scale = uvec::IDENTITY_SCALE;
		};

		void transform(const Vector3 &parentPos, const Quat &parentRot, const Vector3 *optParentScale, Vector3 &pos, Quat &rot, Vector3 *optScale);
	};
	DLLMUTIL Vector3 operator*(const Vector3 &v, const umath::Transform &t);
	DLLMUTIL Vector3 &operator*=(Vector3 &v, const umath::Transform &t);
	DLLMUTIL Quat operator*(const Quat &v, const umath::Transform &t);
	DLLMUTIL Quat &operator*=(Quat &v, const umath::Transform &t);
	DLLMUTIL umath::Transform operator*(float weight, const umath::Transform &t);

	DLLMUTIL Vector3 operator*(const Vector3 &v, const umath::ScaledTransform &t);
	DLLMUTIL Vector3 &operator*=(Vector3 &v, const umath::ScaledTransform &t);
	DLLMUTIL Quat operator*(const Quat &v, const umath::ScaledTransform &t);
	DLLMUTIL Quat &operator*=(Quat &v, const umath::ScaledTransform &t);
	DLLMUTIL umath::ScaledTransform operator*(float weight, const umath::ScaledTransform &t);
}
