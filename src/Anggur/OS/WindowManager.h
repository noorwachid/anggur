#pragma once

namespace Anggur {
	class WindowManager {
	public:
		WindowManager();

		~WindowManager();

		/**
		 * Poll event of each devices
		 */
		void pollEvents();
	};
}
