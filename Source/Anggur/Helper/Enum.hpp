#pragma once

#include <type_traits>

#define Anggur_EnumUnary(sym, op) \
inline sym operator op (sym lhs) \
{ \
    return static_cast<sym>( \
        op static_cast<std::underlying_type<sym>::type>(lhs) \
    ); \
}

#define Anggur_EnumBinary(sym, op) \
inline sym operator op (sym lhs, sym rhs) \
{ \
    using ut = std::underlying_type<sym>::type; \
    return static_cast<sym>( \
        static_cast<ut>(lhs) op static_cast<ut>(rhs) \
    ); \
}

#define Anggur_EnumBinaryAssigment(sym, op) \
inline sym operator op (sym& lhs, sym rhs) \
{ \
    using ut = std::underlying_type<sym>::type; \
    ut l = static_cast<ut>(lhs); \
    ut r = static_cast<ut>(rhs); \
    l op r; \
    lhs = static_cast<sym>(l); \
    return lhs; \
}

#define Anggur_EnumBinaryComparison(sym, op) \
inline bool operator op (sym lhs, sym rhs) \
{ \
    using ut = std::underlying_type<sym>::type; \
    return static_cast<ut>(lhs) op static_cast<ut>(rhs); \
}

#define Anggur_EnumOverloads(sym) \
Anggur_EnumUnary(sym, ~) \
Anggur_EnumBinary(sym, &) \
Anggur_EnumBinary(sym, |) \
Anggur_EnumBinary(sym, ^) \
Anggur_EnumBinaryAssigment(sym, &=) \
Anggur_EnumBinaryAssigment(sym, |=) \
Anggur_EnumBinaryAssigment(sym, ^=) \
Anggur_EnumBinaryComparison(sym, ==) \
Anggur_EnumBinaryComparison(sym, !=)
