#pragma once

namespace Anggur {

class Timer
{
public:
    static float GetDx();
    static float GetElapsed();

private:
    friend class Application;

    static float mDx;
    static float mElapsed;
};

}
