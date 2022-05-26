#pragma once

#include <string>
#include <vector>

namespace Text {
	using Codepoint = uint32_t;

	namespace UTF8 {
		std::vector<Codepoint> Parse(const char* buffer) {
			std::vector<Codepoint> codepoints;
			Codepoint temporaryCodepoint = 0;
			int temporaryLeftover = 0;
			bool start = true;

			for (const char *c = buffer; *c; ++c) {
				if (temporaryLeftover > 0) {
					--temporaryLeftover;
					if ((*c & 0xc0) == 0x80)
						temporaryCodepoint |= (*c & 0x3f) << (6 * temporaryLeftover);
					// else error
				} else if (!(*c & 0x80)) {
					temporaryCodepoint = *c;
					temporaryLeftover = 0;
				} else if (*c&0x40) {
					int block;
					for (block = 0; ((unsigned char) *c << block) & 0x40 && block < 4; ++block);
					if (block < 4) {
						temporaryCodepoint = (*c & (0x3f >> block)) << (6 * block);
						temporaryLeftover = block;
					} else {
						continue; // error
					}
				} else {
					continue; // error
				}

				if (!temporaryLeftover) {
					if (!(start && temporaryCodepoint == 0xfeff)) // BOM
						codepoints.push_back(temporaryCodepoint);
					start = false;
				}
			}

			return codepoints;
		}

		std::string Compose(const std::vector<Codepoint>& codepoints) {
			// TODO: compose the codepoint to utf8 bytes
			return std::string();	
		}
	}
}

