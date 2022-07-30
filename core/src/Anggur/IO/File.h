#pragma once

#include <vector>
#include <string>

namespace Anggur::File 
{
	std::vector<uint8_t> Load(const std::string& path);

	std::string LoadText(const std::string& path);
}
