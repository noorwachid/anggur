#pragma once

#include "Anggur/Integral.h"
#include <string>

namespace Anggur::UTF8
{
	std::string Unpack(uint codepoint);

	uint Pack(const std::string& text, usize index = 0);

	uint PackAndMoveIndex(const std::string& text, usize& index);
}
