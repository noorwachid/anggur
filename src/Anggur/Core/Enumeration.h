#pragma once

#include <type_traits>

#define ANGGUR_ENUMERATION_UNARY(sym, op)                                                                              \
	inline sym operator op(sym lhs)                                                                                    \
	{                                                                                                                  \
		return static_cast<sym>(op static_cast<std::underlying_type<sym>::type>(lhs));                                 \
	}

#define ANGGUR_ENUMERATION_BINARY(sym, op)                                                                             \
	inline sym operator op(sym lhs, sym rhs)                                                                           \
	{                                                                                                                  \
		using ut = std::underlying_type<sym>::type;                                                                    \
		return static_cast<sym>(static_cast<ut>(lhs) op static_cast<ut>(rhs));                                         \
	}

#define ANGGUR_ENUMERATION_ASSIGNMENT(sym, op)                                                                         \
	inline sym operator op(sym& lhs, sym rhs)                                                                          \
	{                                                                                                                  \
		using ut = std::underlying_type<sym>::type;                                                                    \
		ut l = static_cast<ut>(lhs);                                                                                   \
		ut r = static_cast<ut>(rhs);                                                                                   \
		l op r;                                                                                                        \
		lhs = static_cast<sym>(l);                                                                                     \
		return lhs;                                                                                                    \
	}

#define ANGGUR_ENUMERATION_COMPARISON(sym, op)                                                                         \
	inline bool operator op(sym lhs, sym rhs)                                                                          \
	{                                                                                                                  \
		using ut = std::underlying_type<sym>::type;                                                                    \
		return static_cast<ut>(lhs) op static_cast<ut>(rhs);                                                           \
	}

#define ANGGUR_ENUMERATION(sym)                                                                                        \
	ANGGUR_ENUMERATION_UNARY(sym, ~)                                                                                   \
	ANGGUR_ENUMERATION_BINARY(sym, &)                                                                                  \
	ANGGUR_ENUMERATION_BINARY(sym, |)                                                                                  \
	ANGGUR_ENUMERATION_BINARY(sym, ^)                                                                                  \
	ANGGUR_ENUMERATION_ASSIGNMENT(sym, &=)                                                                             \
	ANGGUR_ENUMERATION_ASSIGNMENT(sym, |=)                                                                             \
	ANGGUR_ENUMERATION_ASSIGNMENT(sym, ^=)                                                                             \
	ANGGUR_ENUMERATION_COMPARISON(sym, ==)                                                                             \
	ANGGUR_ENUMERATION_COMPARISON(sym, !=)
