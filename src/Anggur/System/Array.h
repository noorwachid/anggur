#pragma once

#include <vector>

namespace Anggur
{
    template <typename T>
    using Array = std::vector<T>;

    template <typename T, size_t Size>
    using FixedArray = std::array<T, Size>;
}