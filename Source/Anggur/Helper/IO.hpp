#pragma once
#include <string>

namespace Anggur {

struct IO
{
    static uint8_t* Load(const std::string& path, size_t& size);
    static std::string Read(const std::string& path);
    static std::string GetFileExtention(const std::string& path);
};

}
