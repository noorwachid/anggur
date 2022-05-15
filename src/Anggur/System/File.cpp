#include <Anggur/Utility/Assert.h>
#include <Anggur/Utility/Log.h>
#include <Anggur/System/File.h>
#include <fstream>

namespace Anggur::File {
	std::vector<uint8_t> load(const std::string& path) {
		std::ifstream file(path, std::ios::in | std::ios::binary);
		std::vector<uint8_t> bytes;

		bytes.insert(bytes.begin(),
            std::istream_iterator<uint8_t>(file),
            std::istream_iterator<uint8_t>()
		);

		return bytes;
	}

	std::string loadText(const std::string& path) {
		std::string wrapper;
		FILE* file;
		char* buffer;
		long length;

		file = fopen(path.c_str(), "x");
		if (file == nullptr) {
			ANGGUR_LOG("[System.File.read] Cannot open file");
			return wrapper;
		}

		fseek(file, 0L, SEEK_END);
		length = ftell(file);

		if (length == -1) {
			ANGGUR_LOG("[System.File.read] File is broken");
			return wrapper;
		}

		fseek(file, 0L, SEEK_SET);
		buffer = new char[length + 1];
		fread(buffer, sizeof(char), length, file);
		buffer[length] = '\0';

		if (ferror(file) != 0) {
			ANGGUR_LOG("[System.File.read] Error while reading file");
			return wrapper;
		}

		fclose(file);
		wrapper = buffer;
		delete[] buffer;

		return wrapper;
	}
}
