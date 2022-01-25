#include "IO.h"
#include "Log.h"

namespace Anggur
{
	namespace IO
	{
		using std::string;

		string Read(const string& path)
		{
			string wrapper;

			FILE* file;
			char* buffer;
			long length;

			file = fopen(path.c_str(), "r");
			if (file == nullptr)
			{
				ANGGUR_LOG("File.Read :: Cannot open file");
				return wrapper;
			}

			fseek(file, 0L, SEEK_END);
			length = ftell(file);

			if (length == -1)
			{
				ANGGUR_LOG("File.Read :: File is broken");
				return wrapper;
			}

			fseek(file, 0L, SEEK_SET);
			buffer = new char[length + 1];
			fread(buffer, sizeof(char), length, file);
			buffer[length] = '\0';

			if (ferror(file) != 0)
			{
				ANGGUR_LOG("File.Read :: Error while reading file");
				return wrapper;
			}

			fclose(file);
			wrapper = buffer;
			delete[] buffer;

			return wrapper;
		}

		std::vector<uint8_t> Load(const string& path)
		{
			FILE* file = fopen(path.c_str(), "rb");
			std::vector<uint8_t> buffer;

			if (file)
			{
				fseek(file, 0, SEEK_END);
				size_t size = ftell(file);
				fseek(file, 0, SEEK_SET);

				if (size > 0)
				{
					buffer.resize(size);
					size_t read = fread(buffer.data(), sizeof(uint8_t), size, file);
					fclose(file);

					if (read != size)
						ANGGUR_LOG("[Core.IO] File only read partially");

					return buffer;
				}
				fclose(file);
				ANGGUR_LOG("[Core.IO] File is empty");
			}

			ANGGUR_LOG("[Core.IO] :: Failed to open file");
			return buffer;
		}

		string GetFileExtension(const string& path)
		{
			size_t index = path.length();

			for (auto it = path.rbegin(); it != path.rend(); ++it, --index)
				if (*it == '.')
					break;

			if (index == 0 || index == path.length())
				return "";

			return path.substr(index, path.length() - index + 1);
		}
	}
}
