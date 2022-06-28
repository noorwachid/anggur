#include "File.h"
#include <fstream>

namespace Anggur::File 
{
	std::vector<uint8_t> Load(const std::string& path) 
	{
		std::ifstream file(path, std::ios::in | std::ios::binary);
		std::vector<uint8_t> bytes;

		bytes.insert(bytes.begin(),
            std::istream_iterator<uint8_t>(file),
            std::istream_iterator<uint8_t>()
		);

		return bytes;
	}

	std::string LoadText(const std::string& path) 
	{
		std::ifstream file(path, std::ios::in | std::ios::binary);
		std::string text;

		text.insert(text.begin(),
            std::istream_iterator<uint8_t>(file),
            std::istream_iterator<uint8_t>()
		);

		return text;
	}
}
