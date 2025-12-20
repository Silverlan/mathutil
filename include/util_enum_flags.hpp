// SPDX-FileCopyrightText: (c) 2025 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

#pragma once

#define REGISTER_ENUM_FLAGS(ENUM_TYPE) \
    namespace pragma::math::scoped_enum::bitwise { \
        template<> \
        struct enable_bitwise_operators<ENUM_TYPE> : std::true_type {}; \
    } \
    namespace magic_enum::customize { \
        template <> \
        struct enum_range<ENUM_TYPE> { \
            static constexpr bool is_flags = true; \
        }; \
    }

#define REGISTER_ENUM_ARITHMETIC_OPERATORS(ENUM_TYPE) \
	namespace pragma::math::scoped_enum::arithmetic { \
		template<> \
		struct enable_arithmetic_operators<platform::CursorMode> : std::true_type {}; \
	}
