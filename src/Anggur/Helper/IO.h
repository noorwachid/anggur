#pragma once
#include <string>
#include <vector>

using std::string;

namespace Anggur
{

struct IO
{
    static std::vector<uint8_t> Load(const string& path);
    static string Read(const string& path);
    static string GetFileExtention(const string& path);
};

} // namespace Anggur
