// SPDX-FileCopyrightText: (c) 2021 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

#ifndef __UMATH_SCOPED_ENUM_OPERATORS_HPP__
#define __UMATH_SCOPED_ENUM_OPERATORS_HPP__

#include <type_traits>

namespace umath::scoped_enum {
	namespace bitwise {
		template<typename T>
		    requires(std::is_enum_v<T>)
		inline constexpr std::underlying_type_t<T> to_base_type(T v)
		{
			return static_cast<std::underlying_type_t<T>>(v);
		}
		template<typename T>
		    requires(!std::is_enum_v<T>)
		inline constexpr T to_base_type(T v)
		{
			return v;
		}

		template<typename T0, typename T1, typename TScopedEnum = T0>
		inline constexpr TScopedEnum operator|(const T0 &a, const T1 &b)
		{
			return static_cast<TScopedEnum>(to_base_type(a) | to_base_type(b));
		}

		template<typename TScopedEnum, typename T>
		inline TScopedEnum &operator|=(TScopedEnum &a, const T &b)
		{
			a = a | b;
			return a;
		}
		template<typename T0, typename T1, typename TScopedEnum = T0>
		inline constexpr TScopedEnum operator&(const T0 &a, const T1 &b)
		{
			return static_cast<TScopedEnum>(to_base_type(a) & to_base_type(b));
		}
		template<typename TScopedEnum, typename T>
		inline TScopedEnum &operator&=(TScopedEnum &a, const T &b)
		{
			a = a & b;
			return a;
		}
		template<typename T0, typename T1, typename TScopedEnum = T0>
		inline constexpr TScopedEnum operator<<(const T0 &a, const T1 &b)
		{
			return static_cast<TScopedEnum>(to_base_type(a) << to_base_type(b));
		}
		template<typename TScopedEnum, typename T>
		inline TScopedEnum &operator<<=(TScopedEnum &a, const T &b)
		{
			a = a << b;
			return a;
		}
		template<typename T0, typename T1, typename TScopedEnum = T0>
		inline constexpr TScopedEnum operator>>(const T0 &a, const T1 &b)
		{
			return static_cast<TScopedEnum>(to_base_type(a) >> to_base_type(b));
		}
		template<typename TScopedEnum, typename T>
		inline TScopedEnum &operator>>=(TScopedEnum &a, const T &b)
		{
			a = a >> b;
			return a;
		}
		template<typename TScopedEnum>
		inline constexpr TScopedEnum operator~(const TScopedEnum &a)
		{
			return static_cast<TScopedEnum>(~static_cast<std::underlying_type_t<TScopedEnum>>(a));
		}
	};

	namespace arithmetic {
		template<typename T0, typename T1, typename TScopedEnum = T0>
		inline constexpr TScopedEnum operator+(const T0 &a, const T1 &b)
		{
			return static_cast<TScopedEnum>(static_cast<std::underlying_type_t<TScopedEnum>>(a) + static_cast<std::underlying_type_t<TScopedEnum>>(b));
		}
		template<typename TScopedEnum>
		inline constexpr TScopedEnum &operator++(TScopedEnum &a)
		{
			a = static_cast<TScopedEnum>(static_cast<std::underlying_type_t<TScopedEnum>>(a) + 1);
			return a;
		}
		template<typename TScopedEnum>
		inline constexpr TScopedEnum operator++(TScopedEnum &a, int)
		{
			auto r = a;
			operator++(a);
			return r;
		}
		template<typename TScopedEnum, typename T1>
		inline TScopedEnum &operator+=(TScopedEnum &a, const T1 &b)
		{
			a = operator+<TScopedEnum, T1, TScopedEnum>(a, b);
			return a;
		}
		template<typename T0, typename T1, typename TScopedEnum = T0>
		inline constexpr TScopedEnum operator-(const T0 &a, const T1 &b)
		{
			return static_cast<TScopedEnum>(static_cast<std::underlying_type_t<TScopedEnum>>(a) - static_cast<std::underlying_type_t<TScopedEnum>>(b));
		}
		template<typename TScopedEnum>
		inline constexpr TScopedEnum &operator--(TScopedEnum &a)
		{
			a = static_cast<TScopedEnum>(static_cast<std::underlying_type_t<TScopedEnum>>(a) - 1);
			return a;
		}
		template<typename TScopedEnum>
		inline constexpr TScopedEnum operator--(TScopedEnum &a, int)
		{
			auto r = a;
			operator--(a);
			return r;
		}
		template<typename TScopedEnum, typename T1>
		inline TScopedEnum &operator-=(TScopedEnum &a, const T1 &b)
		{
			a = operator-<TScopedEnum, T1, TScopedEnum>(a, b);
			return a;
		}
	};
};

#define REGISTER_BASIC_BITWISE_OPERATORS_ARITHMETIC_TO_SCOPED_ENUM(typeA, typeB, enumType)                                                                                                                                                                                                       \
	inline constexpr enumType operator|(const typeA &a, const typeB &b)                                                                                                                                                                                                                          \
	{                                                                                                                                                                                                                                                                                            \
		return umath::scoped_enum::bitwise::operator|<typeA, typeB, enumType>(a, b);                                                                                                                                                                                                             \
	}                                                                                                                                                                                                                                                                                            \
	inline constexpr enumType operator&(const typeA &a, const typeB &b)                                                                                                                                                                                                                          \
	{                                                                                                                                                                                                                                                                                            \
		return umath::scoped_enum::bitwise::operator&<typeA, typeB, enumType>(a, b);                                                                                                                                                                                                             \
	}                                                                                                                                                                                                                                                                                            \
	inline constexpr enumType operator<<(const typeA &a, const typeB &b)                                                                                                                                                                                                                         \
	{                                                                                                                                                                                                                                                                                            \
		return umath::scoped_enum::bitwise::operator<< <typeA, typeB, enumType>(a, b);                                                                                                                                                                                                           \
	}                                                                                                                                                                                                                                                                                            \
	inline constexpr enumType operator>>(const typeA &a, const typeB &b)                                                                                                                                                                                                                         \
	{                                                                                                                                                                                                                                                                                            \
		return umath::scoped_enum::bitwise::operator>><typeA, typeB, enumType>(a, b);                                                                                                                                                                                                            \
	}

#define REGISTER_BASIC_BITWISE_OPERATORS_SCOPED_ENUM(typeA, typeB, enumType)                                                                                                                                                                                                                     \
	inline constexpr enumType operator|(const typeA &a, const typeB &b)                                                                                                                                                                                                                          \
	{                                                                                                                                                                                                                                                                                            \
		return umath::scoped_enum::bitwise::operator|<typeA, typeB, enumType>(a, b);                                                                                                                                                                                                             \
	}                                                                                                                                                                                                                                                                                            \
	inline enumType &operator|=(typeA &a, const typeB &b)                                                                                                                                                                                                                                        \
	{                                                                                                                                                                                                                                                                                            \
		return umath::scoped_enum::bitwise::operator|=<typeA, typeB>(a, b);                                                                                                                                                                                                                      \
	}                                                                                                                                                                                                                                                                                            \
	inline constexpr enumType operator&(const typeA &a, const typeB &b)                                                                                                                                                                                                                          \
	{                                                                                                                                                                                                                                                                                            \
		return umath::scoped_enum::bitwise::operator&<typeA, typeB, enumType>(a, b);                                                                                                                                                                                                             \
	}                                                                                                                                                                                                                                                                                            \
	inline enumType &operator&=(typeA &a, const typeB &b)                                                                                                                                                                                                                                        \
	{                                                                                                                                                                                                                                                                                            \
		return umath::scoped_enum::bitwise::operator&=<typeA, typeB>(a, b);                                                                                                                                                                                                                      \
	}                                                                                                                                                                                                                                                                                            \
	inline constexpr enumType operator<<(const typeA &a, const typeB &b)                                                                                                                                                                                                                         \
	{                                                                                                                                                                                                                                                                                            \
		return umath::scoped_enum::bitwise::operator<< <typeA, typeB, enumType>(a, b);                                                                                                                                                                                                           \
	}                                                                                                                                                                                                                                                                                            \
	inline enumType &operator<<=(typeA &a, const typeB &b)                                                                                                                                                                                                                                       \
	{                                                                                                                                                                                                                                                                                            \
		return umath::scoped_enum::bitwise::operator<<=<typeA, typeB>(a, b);                                                                                                                                                                                                                     \
	}                                                                                                                                                                                                                                                                                            \
	inline constexpr enumType operator>>(const typeA &a, const typeB &b)                                                                                                                                                                                                                         \
	{                                                                                                                                                                                                                                                                                            \
		return umath::scoped_enum::bitwise::operator>><typeA, typeB, enumType>(a, b);                                                                                                                                                                                                            \
	}                                                                                                                                                                                                                                                                                            \
	inline enumType &operator>>=(typeA &a, const typeB &b)                                                                                                                                                                                                                                       \
	{                                                                                                                                                                                                                                                                                            \
		return umath::scoped_enum::bitwise::operator>>=<typeA, typeB>(a, b);                                                                                                                                                                                                                     \
	}

#define REGISTER_BASIC_BITWISE_OPERATORS(type)                                                                                                                                                                                                                                                   \
	REGISTER_BASIC_BITWISE_OPERATORS_SCOPED_ENUM(type, type, type)                                                                                                                                                                                                                               \
	inline constexpr type operator~(const type &a)                                                                                                                                                                                                                                               \
	{                                                                                                                                                                                                                                                                                            \
		return static_cast<type>(~static_cast<std::underlying_type_t<type>>(a));                                                                                                                                                                                                                 \
	}

#define REGISTER_BASIC_BITWISE_OPERATORS_2WAY(type)                                                                                                                                                                                                                                              \
	REGISTER_BASIC_BITWISE_OPERATORS(type)                                                                                                                                                                                                                                                       \
	REGISTER_BASIC_BITWISE_OPERATORS_SCOPED_ENUM(type, std::underlying_type_t<type>, type)                                                                                                                                                                                                       \
	REGISTER_BASIC_BITWISE_OPERATORS_ARITHMETIC_TO_SCOPED_ENUM(std::underlying_type_t<type>, type, type)

	//////////////

#define REGISTER_BASIC_ARITHMETIC_OPERATORS_SCOPED_ENUM_WITH_ARITHMETIC(typeA, typeB, enumType)                                                                                                                                                                                                  \
	inline constexpr enumType operator+(const typeA &a, const typeB &b)                                                                                                                                                                                                                          \
	{                                                                                                                                                                                                                                                                                            \
		return umath::scoped_enum::arithmetic::operator+<typeA, typeB, enumType>(a, b);                                                                                                                                                                                                          \
	}                                                                                                                                                                                                                                                                                            \
	inline constexpr enumType operator+(const typeB &a, const typeA &b)                                                                                                                                                                                                                          \
	{                                                                                                                                                                                                                                                                                            \
		return umath::scoped_enum::arithmetic::operator+<typeB, typeA, enumType>(a, b);                                                                                                                                                                                                          \
	}                                                                                                                                                                                                                                                                                            \
	inline enumType &operator+=(typeA &a, const typeB &b)                                                                                                                                                                                                                                        \
	{                                                                                                                                                                                                                                                                                            \
		return umath::scoped_enum::arithmetic::operator+=<enumType, typeB>(a, b);                                                                                                                                                                                                                \
	}                                                                                                                                                                                                                                                                                            \
	inline constexpr enumType operator-(const typeA &a, const typeB &b)                                                                                                                                                                                                                          \
	{                                                                                                                                                                                                                                                                                            \
		return umath::scoped_enum::arithmetic::operator-<typeA, typeB, enumType>(a, b);                                                                                                                                                                                                          \
	}                                                                                                                                                                                                                                                                                            \
	inline constexpr enumType operator-(const typeB &a, const typeA &b)                                                                                                                                                                                                                          \
	{                                                                                                                                                                                                                                                                                            \
		return umath::scoped_enum::arithmetic::operator-<typeB, typeA, enumType>(a, b);                                                                                                                                                                                                          \
	}                                                                                                                                                                                                                                                                                            \
	inline enumType &operator-=(typeA &a, const typeB &b)                                                                                                                                                                                                                                        \
	{                                                                                                                                                                                                                                                                                            \
		return umath::scoped_enum::arithmetic::operator-=<enumType, typeB>(a, b);                                                                                                                                                                                                                \
	}

#define REGISTER_BASIC_ARITHMETIC_OPERATORS_SCOPED_ENUM(type)                                                                                                                                                                                                                                    \
	inline constexpr type operator+(const type &a, const type &b)                                                                                                                                                                                                                                \
	{                                                                                                                                                                                                                                                                                            \
		return umath::scoped_enum::arithmetic::operator+<type, type>(a, b);                                                                                                                                                                                                                      \
	}                                                                                                                                                                                                                                                                                            \
	inline constexpr type &operator++(type &a)                                                                                                                                                                                                                                                   \
	{                                                                                                                                                                                                                                                                                            \
		return umath::scoped_enum::arithmetic::operator++<type>(a);                                                                                                                                                                                                                              \
	}                                                                                                                                                                                                                                                                                            \
	inline constexpr type operator++(type &a, int i)                                                                                                                                                                                                                                             \
	{                                                                                                                                                                                                                                                                                            \
		return umath::scoped_enum::arithmetic::operator++<type>(a, i);                                                                                                                                                                                                                           \
	}                                                                                                                                                                                                                                                                                            \
	inline type &operator+=(type &a, const type &b)                                                                                                                                                                                                                                              \
	{                                                                                                                                                                                                                                                                                            \
		return umath::scoped_enum::arithmetic::operator+=<type, type>(a, b);                                                                                                                                                                                                                     \
	}                                                                                                                                                                                                                                                                                            \
	inline constexpr type operator-(const type &a, const type &b)                                                                                                                                                                                                                                \
	{                                                                                                                                                                                                                                                                                            \
		return umath::scoped_enum::arithmetic::operator-<type, type>(a, b);                                                                                                                                                                                                                      \
	}                                                                                                                                                                                                                                                                                            \
	inline constexpr type &operator--(type &a)                                                                                                                                                                                                                                                   \
	{                                                                                                                                                                                                                                                                                            \
		return umath::scoped_enum::arithmetic::operator--<type>(a);                                                                                                                                                                                                                              \
	}                                                                                                                                                                                                                                                                                            \
	inline constexpr type operator--(type &a, int i)                                                                                                                                                                                                                                             \
	{                                                                                                                                                                                                                                                                                            \
		return umath::scoped_enum::arithmetic::operator--<type>(a, i);                                                                                                                                                                                                                           \
	}                                                                                                                                                                                                                                                                                            \
	inline type &operator-=(type &a, const type &b)                                                                                                                                                                                                                                              \
	{                                                                                                                                                                                                                                                                                            \
		return umath::scoped_enum::arithmetic::operator-=<type, type>(a, b);                                                                                                                                                                                                                     \
	}

#define REGISTER_BASIC_ARITHMETIC_OPERATORS(type)                                                                                                                                                                                                                                                \
	REGISTER_BASIC_ARITHMETIC_OPERATORS_SCOPED_ENUM(type);                                                                                                                                                                                                                                       \
	REGISTER_BASIC_BITWISE_OPERATORS(type);

#define REGISTER_BASIC_ARITHMETIC_OPERATORS_2WAY(type)                                                                                                                                                                                                                                           \
	REGISTER_BASIC_ARITHMETIC_OPERATORS(type);                                                                                                                                                                                                                                                   \
	REGISTER_BASIC_ARITHMETIC_OPERATORS_SCOPED_ENUM_WITH_ARITHMETIC(type, std::underlying_type_t<type>, type);

#endif
