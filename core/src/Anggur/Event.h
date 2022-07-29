#pragma once

#include <string>

namespace Anggur 
{
    struct Event 
    {
        std::string name;
        bool consumed = false;

        Event(const std::string& newName): name(newName) 
        {
        }

        virtual ~Event() 
        {
        }
    };
}