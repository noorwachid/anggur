#pragma once

#include <Anggur/Utility/Enum.h>

namespace Anggur 
{
	enum class Key: int 
	{
		Unknown = 0,

		Enter = 40,
		Escape = 41,
		Backspace = 42,
		Tab = 43,
		Space = 44,
		Minus = 45,
		Equal = 46,
		LBracket = 47,
		RBracket = 48,
		Backslash = 49,
		NonUsHash = 50, // Euro
		Semicolon = 51,
		Quote = 52,
		Backtick = 53,
		Comma = 54,
		Period = 55,
		Slash = 56,

		N1 = 30,
		N2 = 31,
		N3 = 32,
		N4 = 33,
		N5 = 34,
		N6 = 35,
		N7 = 36,
		N8 = 37,
		N9 = 38,
		N0 = 39,

		A = 4,
		B = 5,
		C = 6,
		D = 7,
		E = 8,
		F = 9,
		G = 10,
		H = 11,
		I = 12,
		J = 13,
		K = 14,
		L = 15,
		M = 16,
		N = 17,
		O = 18,
		P = 19,
		Q = 20,
		R = 21,
		S = 22,
		T = 23,
		U = 24,
		V = 25,
		W = 26,
		X = 27,
		Y = 28,
		Z = 29,

		CapsLock = 57,

		F1 = 58,
		F2 = 59,
		F3 = 60,
		F4 = 61,
		F5 = 62,
		F6 = 63,
		F7 = 64,
		F8 = 65,
		F9 = 66,
		F10 = 67,
		F11 = 68,
		F12 = 69,

		PrintScreen = 70,
		ScrollLock = 71,
		Pause = 72,
		Insert = 73,
		Home = 74,
		PageUp = 75,
		Delete_ = 76,
		End = 77,
		PageDown = 78,
		Right = 79,
		Left = 80,
		Down = 81,
		Up = 82,

		PadNumLock = 83,

		PadDivide = 84,
		PadMultiply = 85,
		PadMinus = 86,
		PadPlus = 87,
		PadReturn = 88,
		PadPoint = 99,
		PadComma = 133,
		PadEqual = 103,
		PadEqual400 = 134,

		Pad1 = 89,
		Pad2 = 90,
		Pad3 = 91,
		Pad4 = 92,
		Pad5 = 93,
		Pad6 = 94,
		Pad7 = 95,
		Pad8 = 96,
		Pad9 = 97,
		Pad0 = 98,

		Application = 101,
		Power = 102,

		F13 = 104,
		F14 = 105,
		F15 = 106,
		F16 = 107,
		F17 = 108,
		F18 = 109,
		F19 = 110,
		F20 = 111,
		F21 = 112,
		F22 = 113,
		F23 = 114,
		F24 = 115,

		Execute = 116,
		Help = 117,
		Menu = 118,
		Select = 119,
		Stop = 120,
		Redo = 121,
		Undo = 122,
		Cut = 123,
		Copy = 124,
		Paste = 125,
		Find = 126,
		Mute = 127,
		VolumeUp = 128,
		VolumeDown = 129,


		AltErase = 153,
		SysReq = 154,
		Cancel = 155,
		Clear = 156,
		Prior = 157,
		Return2 = 158,
		Separator = 159,
		Out = 160,
		Oper = 161,
		ClearAgain = 162,
		Crsel = 163,
		Exsel = 164,

		ThousandsSeparator = 178,
		DecimalSeparator = 179,
		CurrencyUnit = 180,
		CurrencySubunit = 181,

		Pad00 = 176,
		Pad000 = 177,
		PadLParen = 182,
		PadRParen = 183,
		PadLBrace = 184,
		PadRBrace = 185,
		PadTab = 186,
		PadBackspace = 187,
		PadA = 188,
		PadB = 189,
		PadC = 190,
		PadD = 191,
		PadE = 192,
		PadF = 193,
		PadXor = 194,
		PadPower = 195,
		PadPercent = 196,
		PadLess = 197,
		PadGreater = 198,
		PadAmpersand = 199,
		PadDoubleAmpersand = 200,
		PadVertBar = 201,
		PadDoubleVertBar = 202,
		PadColon = 203,
		PadHash = 204,
		PadSpace = 205,
		PadAt = 206,
		PadExclam = 207,
		PadMemStore = 208,
		PadMemRecall = 209,
		PadMemClear = 210,
		PadMemAdd = 211,
		PadMemSubtract = 212,
		PadMemMultiply = 213,
		PadMemDivide = 214,
		PadPlusMinus = 215,
		PadClear = 216,
		PadClearEntry = 217,
		PadBinary = 218,
		PadOctal = 219,
		PadDecimal = 220,
		PadHexadecimal = 221,

		LControl = 224,
		LShift = 225,
		LAlt = 226,
		LMeta = 227,
		RControl = 228,
		RShift = 229,
		RAlt = 230,
		RMeta = 231,

		AltGr = 257,

		AudioNext = 258,
		AudioPrev = 259,
		AudioStop = 260,
		AudioPlay = 261,
		AudioMute = 262,

		AudioRewind = 285,
		AudioFastForward = 286,

		MediaSelect = 263,
		Www = 264,
		Mail = 265,
		Calculator = 266,
		Computer = 267,
		AcSearch = 268,
		AcHome = 269,
		AcBack = 270,
		AcForward = 271,
		AcStop = 272,
		AcRefresh = 273,
		AcBookmarks = 274,

		BrightnessDown = 275,
		BrightnessUp = 276,
		DisplaySwitch = 277,
		IllumToggle = 278,
		IllumDown = 279,
		IllumUp = 280,

		Eject = 281,
		Sleep = 282,
		App1 = 283,
		App2 = 284,
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