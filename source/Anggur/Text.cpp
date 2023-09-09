#include "Anggur/Text.h"

namespace Anggur::Text::UTF8
{
	std::string Expand(unsigned int codepoint)
	{
		std::string result;

		if (codepoint <= 0x7F)
		{
			// Single-byte character
			result += static_cast<char>(codepoint);
		}
		else if (codepoint <= 0x7FF)
		{
			// Two-byte character
			result += static_cast<char>(0xC0 | ((codepoint >> 6) & 0x1F));
			result += static_cast<char>(0x80 | (codepoint & 0x3F));
		}
		else if (codepoint <= 0xFFFF)
		{
			// Three-byte character
			result += static_cast<char>(0xE0 | ((codepoint >> 12) & 0x0F));
			result += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
			result += static_cast<char>(0x80 | (codepoint & 0x3F));
		}
		else if (codepoint <= 0x10FFFF)
		{
			// Four-byte character
			result += static_cast<char>(0xF0 | ((codepoint >> 18) & 0x07));
			result += static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F));
			result += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
			result += static_cast<char>(0x80 | (codepoint & 0x3F));
		}

		return result;
	}

	unsigned int Collapse(const std::string& text, size_t index)
	{
		// std::string text = "Hello, 🌎!";
		unsigned int codepoint = 0;

		if ((text[index] & 0b10000000) == 0b00000000)
		{
			// Single-bytes
			codepoint = text[index];
		}
		else if ((text[index] & 0b11100000) == 0b11000000)
		{
			// Two-bytes
			codepoint = ((text[index] & 0b00011111) << 6) | (text[index + 1] & 0b00111111);
		}
		else if ((text[index] & 0b11110000) == 0b11100000)
		{
			// Three-bytes
			codepoint = ((text[index] & 0b00001111) << 12) | ((text[index + 1] & 0b00111111) << 6) |
						(text[index + 2] & 0b00111111);
		}
		else if ((text[index] & 0b11111000) == 0b11110000)
		{
			// Four-bytes
			codepoint = ((text[index] & 0b00000111) << 18) | ((text[index + 1] & 0b00111111) << 12) |
						((text[index + 2] & 0b00111111) << 6) | (text[index + 3] & 0b00111111);
		}

		return codepoint;
	}

	unsigned int CollapseAndMoveIndex(const std::string& text, size_t& index)
	{
		// std::string text = "Hello, 🌎!";
		unsigned int codepoint = 0;

		if ((text[index] & 0b10000000) == 0b00000000)
		{
			// Single-bytes
			codepoint = text[index];
			++index;
		}
		else if ((text[index] & 0b11100000) == 0b11000000)
		{
			// Two-bytes
			codepoint = ((text[index] & 0b00011111) << 6) | (text[index + 1] & 0b00111111);
			index += 2;
		}
		else if ((text[index] & 0b11110000) == 0b11100000)
		{
			// Three-bytes
			codepoint = ((text[index] & 0b00001111) << 12) | ((text[index + 1] & 0b00111111) << 6) |
						(text[index + 2] & 0b00111111);
			index += 3;
		}
		else if ((text[index] & 0b11111000) == 0b11110000)
		{
			// Four-bytes
			codepoint = ((text[index] & 0b00000111) << 18) | ((text[index + 1] & 0b00111111) << 12) |
						((text[index + 2] & 0b00111111) << 6) | (text[index + 3] & 0b00111111);
			index += 4;
		}
		else
		{
			// More than 4 bytes encoding is not supported
			// Skipping this byte
			++index;
		}

		return codepoint;
	}
}
