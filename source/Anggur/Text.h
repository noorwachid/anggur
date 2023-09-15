#pragma once

#include <string>

namespace Anggur::Text::UTF8 {
	std::string expand(unsigned int codepoint);

	unsigned int collapse(const std::string& text, size_t index = 0);

	unsigned int collapseAndMoveIndex(const std::string& text, size_t& index);
}
