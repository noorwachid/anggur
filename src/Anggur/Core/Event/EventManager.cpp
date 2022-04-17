#include <Anggur/Core/Internal.h>
#include <Anggur/Core/Window.h>
#include <Anggur/Core/Application.h>
#include <Anggur/Core/Event/EventManager.h>
#include <Anggur/Core/Hierarchy/Tree.h>
#include <Anggur/Graphic/Renderer.h>

namespace Anggur {
	void EventManager::attach(Window* window) {

		glfwSetWindowSizeCallback(window->getHandler(), [](WindowHandler* handler, int width, int height) {
			WindowSizeEvent event(Vector2(width, height));
			Tree::emit(event);
		});

		glfwSetFramebufferSizeCallback(window->getHandler(), [](WindowHandler* handler, int width, int height) {
			FrameBufferSizeEvent event(Vector2(width, height));
			Renderer::setViewport(event.size);
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