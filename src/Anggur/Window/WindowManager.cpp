#include <Anggur/Window/Internal.h>
#include <Anggur/Window/WindowManager.h>
#include <Anggur/Window/Event/Common.h>
#include <Anggur/Utility/Assert.h>
#include <Anggur/Utility/Log.h>

namespace Anggur 
{
	struct WindowManagerData 
	{
		std::vector<std::shared_ptr<Window>> windows;
		bool successful = false;

		WindowManagerData()
		{
			#ifdef ANGGUR_OS_APPLE
				glfwInitHint(GLFW_COCOA_CHDIR_RESOURCES, GL_FALSE);
			#endif

			successful = glfwInit();
			ANGGUR_ASSERT(successful, "[Window.WindowManager] Failed to io system");
		}

		~WindowManagerData()
		{
			if (successful) 
			{
				glfwTerminate();
			}	
		}
	};

	std::unique_ptr<WindowManagerData> data = nullptr;

	void WindowManager::Initialize() 
	{
		data = std::make_unique<WindowManagerData>();
	}

	void WindowManager::Add(const std::shared_ptr<Window>& window)
	{
		data->windows.push_back(window);
	}

	std::shared_ptr<Window>& WindowManager::At(size_t index)
	{
		return data->windows[index];
	}

	bool WindowManager::IsOpen()
	{
		size_t size = 0;

		for (auto& window: data->windows) 
		{
			size += window->IsOpen();
		}

		return size;
	}

	void WindowManager::PollEvents() 
	{
		glfwPollEvents();
	}

	void WindowManager::SwapBuffers() 
	{
		for (auto& window: data->windows)
		{
			window->SwapBuffers();
		}
	}
}
