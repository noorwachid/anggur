#pragma once

#include <type_traits>

#define ANGGUR_ENUMERATIONFLAG_BOOLEAN(sym)                                                                            \
	inline bool operator!(sym lhs) {                                                                                   \
		return static_cast<std::underlying_type<sym>::type>(lhs) == 0;                                                 \
	}

#define ANGGUR_ENUMERATIONFLAG_UNARY(sym, op)                                                                          \
	inline sym operator op(sym lhs) {                                                                                  \
		return static_cast<sym>(op static_cast<std::underlying_type<sym>::type>(lhs));                                 \
	}

#define ANGGUR_ENUMERATIONFLAG_BINARY(sym, op)                                                                         \
	inline sym operator op(sym lhs, sym rhs) {                                                                         \
		using ut = std::underlying_type<sym>::type;                                                                    \
		return static_cast<sym>(static_cast<ut>(lhs) op static_cast<ut>(rhs));                                         \
	}

#define ANGGUR_ENUMERATIONFLAG_ASSIGNMENT(sym, op)                                                                     \
	inline sym operator op(sym& lhs, sym rhs) {                                                                        \
		using ut = std::underlying_type<sym>::type;                                                                    \
		ut l = static_cast<ut>(lhs);                                                                                   \
		ut r = static_cast<ut>(rhs);                                                                                   \
		l op r;                                                                                                        \
		lhs = static_cast<sym>(l);                                                                                     \
		return lhs;                                                                                                    \
	}

#define ANGGUR_ENUMERATIONFLAG_COMPARISON(sym, op)                                                                     \
	inline bool operator op(sym lhs, sym rhs) {                                                                        \
		using ut = std::underlying_type<sym>::type;                                                                    \
		return static_cast<ut>(lhs) op static_cast<ut>(rhs);                                                           \
	}

#define ANGGUR_ENUMERATIONFLAG(sym)                                                                                    \
	ANGGUR_ENUMERATIONFLAG_BOOLEAN(sym)                                                                                \
	ANGGUR_ENUMERATIONFLAG_UNARY(sym, ~)                                                                               \
	ANGGUR_ENUMERATIONFLAG_BINARY(sym, &)                                                                              \
	ANGGUR_ENUMERATIONFLAG_BINARY(sym, |)                                                                              \
	ANGGUR_ENUMERATIONFLAG_BINARY(sym, ^)                                                                              \
	ANGGUR_ENUMERATIONFLAG_ASSIGNMENT(sym, &=)                                                                         \
	ANGGUR_ENUMERATIONFLAG_ASSIGNMENT(sym, |=)                                                                         \
	ANGGUR_ENUMERATIONFLAG_ASSIGNMENT(sym, ^=)                                                                         \
	ANGGUR_ENUMERATIONFLAG_COMPARISON(sym, ==)                                                                         \
	ANGGUR_ENUMERATIONFLAG_COMPARISON(sym, !=)
