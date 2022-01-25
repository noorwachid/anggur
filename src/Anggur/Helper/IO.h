#pragma once
#include <string>
#include <vector>

namespace Anggur
{
	namespace IO
	{
		using std::string;

		std::vector<uint8_t> Load(const string& path);
		string Read(const string& path);
		string GetFileExtension(const string& path);
	};
}
