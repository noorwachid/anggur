#include <Anggur/Window/Internal.h>
#include <Anggur/Window/WindowManager.h>
#include <Anggur/Window/Event/Common.h>
#include <Anggur/Utility/Assert.h>

namespace Anggur {
	WindowManager::WindowManager() {
		successful = glfwInit();
		ANGGUR_ASSERT(successful, "[Window.WindowManager] Failed to io system");
	}

	WindowManager::~WindowManager() {
		if (successful) {
			glfwTerminate();
		}
	}

	void WindowManager::pollEvents() {
		glfwPollEvents();
	}
}
