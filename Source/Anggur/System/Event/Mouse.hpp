#pragma once

namespace Anggur {

enum class Mouse
{
    F1 = 0x001,
    F2 = 0x002,
    F3 = 0x004,
    F4 = 0x008,
    F5 = 0x010,
    
    // Aliases
    Left   = Mouse::F1,
    Middle = Mouse::F2,
    Right  = Mouse::F3,
};

}
