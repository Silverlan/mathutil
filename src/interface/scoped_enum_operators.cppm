// SPDX-FileCopyrightText: (c) 2025 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

module;


export module pragma.math:bitmask_ops;

export import std.compat;

export namespace umath::scoped_enum {
	template<typename E>
		concept ScopedEnum = std::is_enum_v<E>;
	namespace bitwise {
		template<typename E>
			struct enable_bitwise_operators : std::false_type {};

		template<typename E>
			inline constexpr bool enable_bitwise_operators_v = enable_bitwise_operators<E>::value;

		template<typename E>
			concept BitwiseEnum = (std::is_enum_v<E> && enable_bitwise_operators_v<E>) || (!std::is_enum_v<E> && std::is_integral_v<E>);

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

		template<BitwiseEnum T0, BitwiseEnum T1, ScopedEnum TScopedEnum = T0>
		inline constexpr TScopedEnum operator|(const T0 &a, const T1 &b)
		{
			return static_cast<TScopedEnum>(to_base_type(a) | to_base_type(b));
		}

		template<BitwiseEnum TScopedEnum, BitwiseEnum T>
		inline TScopedEnum &operator|=(TScopedEnum &a, const T &b)
		{
			a = a | b;
			return a;
		}
		template<BitwiseEnum T0, BitwiseEnum T1, ScopedEnum TScopedEnum = T0>
		inline constexpr TScopedEnum operator&(const T0 &a, const T1 &b)
		{
			return static_cast<TScopedEnum>(to_base_type(a) & to_base_type(b));
		}
		template<BitwiseEnum TScopedEnum, BitwiseEnum T>
		inline TScopedEnum &operator&=(TScopedEnum &a, const T &b)
		{
			a = a & b;
			return a;
		}
		template<BitwiseEnum T0, BitwiseEnum T1, ScopedEnum TScopedEnum = T0>
		inline constexpr TScopedEnum operator<<(const T0 &a, const T1 &b)
		{
			return static_cast<TScopedEnum>(to_base_type(a) << to_base_type(b));
		}
		template<BitwiseEnum TScopedEnum, BitwiseEnum T>
		inline TScopedEnum &operator<<=(TScopedEnum &a, const T &b)
		{
			a = a << b;
			return a;
		}
		template<BitwiseEnum T0, BitwiseEnum T1, ScopedEnum TScopedEnum = T0>
		inline constexpr TScopedEnum operator>>(const T0 &a, const T1 &b)
		{
			return static_cast<TScopedEnum>(to_base_type(a) >> to_base_type(b));
		}
		template<BitwiseEnum TScopedEnum, BitwiseEnum T>
		inline TScopedEnum &operator>>=(TScopedEnum &a, const T &b)
		{
			a = a >> b;
			return a;
		}
		template<BitwiseEnum TScopedEnum>
		inline constexpr TScopedEnum operator~(const TScopedEnum &a)
		{
			return static_cast<TScopedEnum>(~static_cast<std::underlying_type_t<TScopedEnum>>(a));
		}
	};

	namespace arithmetic {
		template<typename E>
			struct enable_arithmetic_operators : std::false_type {};

		template<typename E>
			inline constexpr bool enable_arithmetic_operators_v = enable_arithmetic_operators<E>::value;

		template<typename E>
			concept ArtihmeticEnum = (std::is_enum_v<E> && enable_arithmetic_operators_v<E>) || (!std::is_enum_v<E> && std::is_integral_v<E>);

		template<ArtihmeticEnum T0, ArtihmeticEnum T1, ScopedEnum TScopedEnum = T0>
		inline constexpr TScopedEnum operator+(const T0 &a, const T1 &b)
		{
			return static_cast<TScopedEnum>(static_cast<std::underlying_type_t<TScopedEnum>>(a) + static_cast<std::underlying_type_t<TScopedEnum>>(b));
		}
		template<ArtihmeticEnum TScopedEnum>
		inline constexpr TScopedEnum &operator++(TScopedEnum &a)
		{
			a = static_cast<TScopedEnum>(static_cast<std::underlying_type_t<TScopedEnum>>(a) + 1);
			return a;
		}
		template<ArtihmeticEnum TScopedEnum>
		inline constexpr TScopedEnum operator++(TScopedEnum &a, int)
		{
			auto r = a;
			operator++(a);
			return r;
		}
		template<ArtihmeticEnum TScopedEnum, ArtihmeticEnum T1>
		inline TScopedEnum &operator+=(TScopedEnum &a, const T1 &b)
		{
			a = operator+<TScopedEnum, T1, TScopedEnum>(a, b);
			return a;
		}
		template<ArtihmeticEnum T0, ArtihmeticEnum T1, ScopedEnum TScopedEnum = T0>
		inline constexpr TScopedEnum operator-(const T0 &a, const T1 &b)
		{
			return static_cast<TScopedEnum>(static_cast<std::underlying_type_t<TScopedEnum>>(a) - static_cast<std::underlying_type_t<TScopedEnum>>(b));
		}
		template<ArtihmeticEnum TScopedEnum>
		inline constexpr TScopedEnum &operator--(TScopedEnum &a)
		{
			a = static_cast<TScopedEnum>(static_cast<std::underlying_type_t<TScopedEnum>>(a) - 1);
			return a;
		}
		template<ArtihmeticEnum TScopedEnum>
		inline constexpr TScopedEnum operator--(TScopedEnum &a, int)
		{
			auto r = a;
			operator--(a);
			return r;
		}
		template<ArtihmeticEnum TScopedEnum, ArtihmeticEnum T1>
		inline TScopedEnum &operator-=(TScopedEnum &a, const T1 &b)
		{
			a = operator-<TScopedEnum, T1, TScopedEnum>(a, b);
			return a;
		}
	};
};

export {
	using namespace umath::scoped_enum;
	using namespace umath::scoped_enum::arithmetic;
	using namespace umath::scoped_enum::bitwise;
}
