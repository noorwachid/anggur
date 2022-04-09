#pragma once
#include <string>
#include <vector>

namespace Anggur {
	namespace IO {
		std::vector<uint8_t> load(const std::string& path);
		std::string read(const std::string& path);
	};
}
