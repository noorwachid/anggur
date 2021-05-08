#pragma once

namespace Anggur {

class Core
{
public:
    static void Initialize();
    static void Terminate();
    static void SetVsync(bool enable);

    static void LoadGlFunctions();
};

}

