#include "Anggur/IO/File.h"
#include <fstream>

namespace Anggur::File
{
	std::vector<uint8_t> Read(const std::string& path)
	{
		std::ifstream file(path, std::ios::in | std::ios::binary);
		std::vector<uint8_t> bytes;

		bytes.insert(bytes.begin(), std::istream_iterator<uint8_t>(file), std::istream_iterator<uint8_t>());

		return bytes;
	}

	std::string ReadText(const std::string& path)
	{
		std::ifstream file(path, std::ios::in | std::ios::binary);
		std::string text;

		text.insert(text.begin(), std::istream_iterator<uint8_t>(file), std::istream_iterator<uint8_t>());

		return text;
	}
}
