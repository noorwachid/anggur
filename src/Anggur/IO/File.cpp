#include "Anggur/IO/File.h"
#include "Anggur/Common/Integral.h"
#include <fstream>

namespace Anggur::File
{
	std::vector<uchar> Read(const std::string& path)
	{
		std::vector<uchar> bytes;
		FILE* fontFile = fopen(path.c_str(), "rb");

		fseek(fontFile, 0, SEEK_END);
		size_t byteSize = ftell(fontFile);
		fseek(fontFile, 0, SEEK_SET);

		bytes.resize(byteSize);

		fread(bytes.data(), byteSize, 1, fontFile);
		fclose(fontFile);

		return bytes;
	}

	std::string ReadText(const std::string& path)
	{
		std::ifstream file(path, std::ios::in | std::ios::binary);
		std::string text;

		if (!file.is_open()) {
			throw std::runtime_error(path + " cannot be read");
		}

		text.insert(text.begin(), std::istream_iterator<uchar>(file), std::istream_iterator<uchar>());

		return text;
	}
}
