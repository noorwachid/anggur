#pragma once

#include <chrono>

namespace Anggur
{
    using ClockContext = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<ClockContext>;

    class Clock
    {
      public:
        Clock()
        {
            started = ClockContext::now();
        }

        /**
         * Get elapsed time on call
         */
        float GetElapsed() const
        {
            TimePoint current = ClockContext::now();
            return std::chrono::duration_cast<std::chrono::milliseconds>(current - started).count() * 0.001;
        }

      private:
        ClockContext context;
        TimePoint started;
    };
}