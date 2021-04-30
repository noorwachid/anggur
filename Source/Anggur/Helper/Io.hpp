#pragma once
#include "Primitive.hpp"
#include <string>

namespace Anggur {

struct Io
{
    static uchar* Load(const std::string& path, size_t& size);
    static std::string Read(const std::string& path);
    static std::string GetFileExtention(const std::string& path);
};

}
