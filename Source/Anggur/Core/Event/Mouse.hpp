#pragma once

namespace Anggur {

enum class MouseButton
{
    F1 = 0x001,
    F2 = 0x002,
    F3 = 0x004,
    F4 = 0x008,
    F5 = 0x010,

    // Aliases
    Left   = MouseButton::F1,
    Middle = MouseButton::F2,
    Right  = MouseButton::F3,
};

}
