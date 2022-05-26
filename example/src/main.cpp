#include <Anggur/Scene/SceneManager.h>
#include "FirstScene.h"


int main() {
	using namespace Anggur;

	WindowManager::initialize();
	WindowManager::set(std::make_shared<Window>(Vector2(800, 600), "Anggur Demo"));

	Renderer::initialize();
	
	SceneManager::set(std::make_shared<FirstScene>());

	while (WindowManager::get()->isOpen()) {
		if (SceneManager::get()) 
			SceneManager::get()->update();

		WindowManager::get()->swapBuffers();
		WindowManager::pollEvents();
	}
}
