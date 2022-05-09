#pragma once

#include <Anggur/Window/Window.h>

namespace Anggur {
	class WindowManager {
	public:
		WindowManager();
		~WindowManager();

		void pollEvents();

	private:
		bool successful;
	};
}
