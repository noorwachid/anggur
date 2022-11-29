#pragma once

namespace Anggur
{
    class WindowSystem
    {
      public:
        WindowSystem();

        ~WindowSystem();

        /**
         * Poll event of each devices
         */
        void PollEvents();
    };
}
