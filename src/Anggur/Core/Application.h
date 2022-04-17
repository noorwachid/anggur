#pragma once

#include <Anggur/Core/Window.h>
#include <Anggur/Core/Hierarchy/Node.h>

namespace Anggur {
	class Application {
	public:
		static void initialize(const Vector2& windowSize, const std::string& windowTitle);
		static void terminate();

		static void run(Node* rootNode);

		static void setVsync(bool enable = true);

		static Window* getWindow();

	private:
		Application();

	private:
		static Window* window;
	};
}
