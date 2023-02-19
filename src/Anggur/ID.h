#pragma once 

#include <cstddef>

namespace Anggur 
{
    using ID = size_t;
    
    ID GenerateID();

    template <typename T>
    ID GetID()
    {
        static ID id = GenerateID();

        return id;
    }
}
