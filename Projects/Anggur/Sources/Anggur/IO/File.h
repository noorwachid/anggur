#pragma once

#include <string>
#include <vector>

namespace Anggur::File
{
    /**
     * Load path as array of buffer
     */
    std::vector<uint8_t> load(const std::string& path);

    /**
     * Load path of to std::string
     */
    std::string loadText(const std::string& path);
}
