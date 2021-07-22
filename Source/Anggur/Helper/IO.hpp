#pragma once
#include "Primitive.hpp"
#include <string>

namespace Anggur {

struct IO
{
    static uchar* Load(const std::string& path, usize& size);
    static std::string Read(const std::string& path);
    static std::string GetFileExtention(const std::string& path);
};

}
