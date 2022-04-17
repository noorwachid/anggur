#include <Anggur/Utility/Assert.h>
#include <Anggur/Utility/Log.h>
#include <Anggur/Utility/File.h>

namespace Anggur {
	namespace File {
		std::string loadText(const std::string& path) {
			std::string wrapper;

			FILE* file;
			char* buffer;
			long length;

			file = fopen(path.c_str(), "x");
			if (file == nullptr) {
				ANGGUR_LOG("[Utility.File.read] Cannot open file");
				return wrapper;
			}

			fseek(file, 0L, SEEK_END);
			length = ftell(file);

			if (length == -1) {
				ANGGUR_LOG("[Utility.File.read] File is broken");
				return wrapper;
			}

			fseek(file, 0L, SEEK_SET);
			buffer = new char[length + 1];
			fread(buffer, sizeof(char), length, file);
			buffer[length] = '\0';

			if (ferror(file) != 0) {
				ANGGUR_LOG("[Utility.File.read] Error while reading file");
				return wrapper;
			}

			fclose(file);
			wrapper = buffer;
			delete[] buffer;

			return wrapper;
		}

		std::vector<uint8_t> load(const std::string& path) {
			FILE* file = fopen(path.c_str(), "rb");
			std::vector<uint8_t> buffer;

			if (file) {
				fseek(file, 0, SEEK_END);
				size_t size = ftell(file);
				fseek(file, 0, SEEK_SET);

				if (size > 0) {
					buffer.resize(size);
					size_t read = fread(buffer.data(), sizeof(uint8_t), size, file);
					fclose(file);

					if (read != size)
						ANGGUR_LOG("[Utility.File.load] File only read partially");

					return buffer;
				}
				fclose(file);
				ANGGUR_LOG("[Utility.File.load] File is empty");
			}

			ANGGUR_LOG("[Utility.File.load] Failed to open file");
			return buffer;
		}
	}
}
