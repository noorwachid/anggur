#include "Anggur/IO/File.h"
#include "Anggur/Integral.h"
#include <fstream>

namespace Anggur::File
{
	std::vector<uchar> Read(const std::string& path)
	{
		std::ifstream file(path, std::ios::in | std::ios::binary);
		std::vector<uchar> bytes;

		if (!file.is_open()) {
			throw std::runtime_error(path + " cannot be read");
		}

		bytes.insert(bytes.begin(), std::istream_iterator<uint8_t>(file), std::istream_iterator<uint8_t>());

		return bytes;
	}

	std::string ReadText(const std::string& path)
	{
		std::ifstream file(path, std::ios::in | std::ios::binary);
		std::string text;

		if (!file.is_open()) {
			throw std::runtime_error(path + " cannot be read");
		}

		text.insert(text.begin(), std::istream_iterator<uint8_t>(file), std::istream_iterator<uint8_t>());

		return text;
	}
}
