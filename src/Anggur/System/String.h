#pragma once

#include <string>

namespace Anggur 
{
    using String = std::string;

    template <typename T>
    String ToString(T value) 
    {
        return std::to_string(value);
    }
}