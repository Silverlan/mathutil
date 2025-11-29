// SPDX-FileCopyrightText: (c) 2025 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

#pragma once

#define REGISTER_ENUM_FLAGS(ENUM_TYPE) \
    namespace umath::scoped_enum::bitwise { \
        template<> \
        struct enable_bitwise_operators<ENUM_TYPE> : std::true_type {}; \
    } \
    namespace magic_enum::customize { \
        template <> \
        struct enum_range<ENUM_TYPE> { \
            static constexpr bool is_flags = true; \
        }; \
    }
