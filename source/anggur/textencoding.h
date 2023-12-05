#pragma once

#include <string>

namespace Anggur::TextEncoding::UTF8
{
	std::string Expand(unsigned int codepoint);

	unsigned int Collapse(const std::string& text, size_t index = 0);

	unsigned int CollapseAndMoveIndex(const std::string& text, size_t& index);
}
