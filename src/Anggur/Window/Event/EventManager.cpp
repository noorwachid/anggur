#include <Anggur/Window/Internal.h>
#include <Anggur/Window/Window.h>
#include <Anggur/Window/Event/EventManager.h>
#include <Anggur/Graphics/Renderer.h>
#include <Anggur/Core/Application.h>
#include <Anggur/Core/Tree.h>

namespace Anggur {
	void EventManager::attach(Window* window) {

		glfwSetWindowSizeCallback(window->getHandler(), [](WindowHandler* handler, int width, int height) {
			WindowSizeEvent event(Vector2(width, height));
			Tree::emit(event);
		});

		glfwSetFramebufferSizeCallback(window->getHandler(), [](WindowHandler* handler, int width, int height) {
			FrameBufferSizeEvent event(Vector2(width, height));
			Tree::emit(event);
		});

		glfwSetCharCallback(window->getHandler(), [](WindowHandler* handler, uint32_t codepoint) {
			CodepointEvent event(codepoint);
			Tree::emit(event);
		});
	}

	void EventManager::detach(Window* window) {

	}

	void EventManager::poll() {
		glfwPollEvents();
	}
}