#pragma once

#include "Anggur/Common/Integral.h"
#include <string>

namespace Anggur::UTF8
{
	std::string Expand(uint codepoint);

	uint Collapse(const std::string& text, usize index = 0);

	uint CollapseAndMoveIndex(const std::string& text, usize& index);
}
