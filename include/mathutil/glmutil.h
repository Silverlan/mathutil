/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __UMATH_GLMUTIL_H__
#define __UMATH_GLMUTIL_H__

#include "mathutildefinitions.h"

#ifndef GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#endif

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif

#ifndef GLM_FORCE_CTOR_INIT
#define GLM_FORCE_CTOR_INIT
#endif

#pragma warning(disable: 4201)
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
#pragma warning(default: 4201)

using Mat2 = glm::mat2x2;
using Mat2x2 = glm::mat2x2;
using Mat2x3 = glm::mat2x3;
using Mat2x4 = glm::mat2x4;
using Mat3x2 = glm::mat3x2;
using Mat3 = glm::mat3x3;
using Mat3x3 = glm::mat3x3;
using Mat3x4 = glm::mat3x4;
using Mat4x2 = glm::mat4x2;
using Mat4x3 = glm::mat4x3;
using Mat4 = glm::mat4x4;
using Mat4x4 = glm::mat4x4;

using Vector2 = glm::vec2;
using Vector3 = glm::vec3;
using Vector4 = glm::vec4;

using Vector2i = glm::ivec2;
using Vector3i = glm::ivec3;
using Vector4i = glm::ivec4;

using Quat = glm::quat;

#endif