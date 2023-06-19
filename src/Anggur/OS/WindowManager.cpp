#include "Anggur/OS/WindowManager.h"
#include "Anggur/Core/Instrumentation.h"
#include "Anggur/OS/Internal.h"
#include <cassert>

namespace Anggur {
	WindowManager::WindowManager() {
#ifdef ANGGUR_OS_X
		glfwInitHint(GLFW_COCOA_CHDIR_RESOURCES, 0);
#endif

		int result = glfwInit();
		assert(result && "[WindowManager] Failed to initalize window system");
	}

	WindowManager::~WindowManager() {
		glfwTerminate();
	}

	void WindowManager::pollEvents() {
		ANGGUR_INSTRUMENTATION_PROFILE_FUNCTION;

		glfwPollEvents();
	}
}
