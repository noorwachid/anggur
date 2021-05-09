#include "Timer.hpp"

namespace Anggur {

float Timer::mDx = 0;
float Timer::mElapsed = 0;

float Timer::GetDx()
{
    return mDx;
}

float Timer::GetElapsed()
{
    return mElapsed;
}

}
