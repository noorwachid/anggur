#include <Anggur/Window/Internal.h>
#include <Anggur/Window/WindowManager.h>
#include <Anggur/Utility/Assert.h>
#include <Anggur/Utility/Log.h>

namespace Anggur {
	struct WindowManagerData {
		bool successful = false;
		std::shared_ptr<Window> window;

		WindowManagerData() {
			#ifdef ANGGUR_OS_APPLE
				glfwInitHint(GLFW_COCOA_CHDIR_RESOURCES, GL_FALSE);
			#endif

			successful = glfwInit();
			ANGGUR_ASSERT(successful, "[Window.WindowManager] Failed to io system");
		}

		~WindowManagerData() {
			if (successful) {
				glfwTerminate();
			}	
		}
	};

	std::unique_ptr<WindowManagerData> data = nullptr;

	void WindowManager::initialize() {
		data = std::make_unique<WindowManagerData>();
	}

	void WindowManager::pollEvents() {
		glfwPollEvents();
	}

	void WindowManager::set(const std::shared_ptr<Window>& window) {
		data->window = window;
	}

	const std::shared_ptr<Window>& WindowManager::get() {
		return data->window;
	}
}
