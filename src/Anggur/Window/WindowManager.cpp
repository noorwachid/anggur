#include <Anggur/Window/Internal.h>
#include <Anggur/Window/WindowManager.h>
#include <Anggur/Window/Event/Common.h>
#include <Anggur/Utility/Assert.h>
#include <Anggur/Utility/Log.h>

namespace Anggur 
{
	WindowManager::WindowManager() 
	{
		#ifdef ANGGUR_OS_APPLE
			glfwInitHint(GLFW_COCOA_CHDIR_RESOURCES, GL_FALSE);
		#endif

		successful = glfwInit();
		ANGGUR_ASSERT(successful, "[Window.WindowManager] Failed to io system");
	}

	WindowManager::~WindowManager() 
	{
		if (successful) 
		{
			glfwTerminate();
		}
	}

	void WindowManager::PollEvents() 
	{
		glfwPollEvents();
	}
}
