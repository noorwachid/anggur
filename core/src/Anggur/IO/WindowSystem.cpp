#include "WindowSystem.h"
#include "Internal.h"
#include <cassert>

namespace Anggur 
{
    WindowSystem::WindowSystem() 
	{
		#ifdef ANGGUR_OS_X
			glfwInitHint(GLFW_COCOA_CHDIR_RESOURCES, 0);
		#endif

		int result = glfwInit();
		assert(result && "[WindowSystem] Failed to initalize window system");
	}

    WindowSystem::~WindowSystem()
    {
        glfwTerminate();
    }

	void WindowSystem::PollEvents()
	{
		glfwPollEvents();
	}
}
