#pragma once

namespace Anggur {

enum class Button
{
    F1 = 0x001,
    F2 = 0x002,
    F3 = 0x004,
    F4 = 0x008,
    F5 = 0x010,

    // Aliases
    Left   = Button::F1,
    Middle = Button::F2,
    Right  = Button::F3,
};

}
