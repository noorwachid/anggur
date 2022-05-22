#pragma once

#include <Anggur/Window/Window.h>

namespace Anggur 
{
	class WindowManager 
	{
	public:
		static void Initialize();
		static void Add(const std::shared_ptr<Window>& window);
		static std::shared_ptr<Window>& At(size_t index);

		static void PollEvents();
		static void SwapBuffers();

		static bool IsOpen();
		static void Close();

	private:
		WindowManager();
	};
}
