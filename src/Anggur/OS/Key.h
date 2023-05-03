#pragma once

#include "Anggur/Common/Enum.h"

namespace Anggur
{
	constexpr int maxKeyValue = 349;

	enum class Key : int
	{
		Unknown = -1,
		Space = 32,
		Apostrophe = 39,
		Comma = 44,
		Minus = 45,
		Period = 46,
		Slash = 47,

		N0 = 48,
		N1 = 49,
		N2 = 50,
		N3 = 51,
		N4 = 52,
		N5 = 53,
		N6 = 54,
		N7 = 55,
		N8 = 56,
		N9 = 57,

		Semicolon = 59,
		Equal = 61,

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftLracket = 91,
		Backslash = 92,
		RightBracket = 93,
		Backtick = 96,
		World1 = 161,
		World2 = 162,

		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		Page_up = 266,
		Page_down = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,

		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		Pad0 = 320,
		Pad1 = 321,
		Pad2 = 322,
		Pad3 = 323,
		Pad4 = 324,
		Pad5 = 325,
		Pad6 = 326,
		Pad7 = 327,
		Pad8 = 328,
		Pad9 = 329,

		PadDecimal = 330,
		PadDivide = 331,
		PadMultiply = 332,
		PadSubtract = 333,
		PadAdd = 334,
		PadEnter = 335,
		PadEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,

		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,

		Menu = 348,
	};

	enum class ModifierKey : unsigned int
	{
		None = 0x0000,
		Shift = 0x0001,
		Control = 0x0002,
		Alt = 0x0004,
		Super = 0x0008,
		CapsLock = 0x0010,
		NumLock = 0x0020,
	};

	ANGGUR_ENUM(ModifierKey);
}
