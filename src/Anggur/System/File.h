#pragma once

#include <vector>
#include <string>

namespace Anggur::File {
	std::vector<uint8_t> load(const std::string& path);
	std::string loadText(const std::string& path);
}
