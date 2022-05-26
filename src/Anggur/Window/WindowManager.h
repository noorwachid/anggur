#pragma once

#include <Anggur/Window/Window.h>
#include <memory>

namespace Anggur {
	class WindowManager {
	public:
		static void initialize();
		static void set(const std::shared_ptr<Window>& window);

		static const std::shared_ptr<Window>& get();

		static void pollEvents();
		static void swapBuffers();

	private:
		WindowManager();
	};
}
