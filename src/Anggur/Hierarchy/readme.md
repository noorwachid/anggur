# Hierarchy

Layer
	std::string name;
	Tree tree;

Scene
	Vector<Layer> layers;

SceneManager
	Scene activeScene; // pointer to active scene








class MainScene: public Scene {
	void initialize() {
	}

	void update() {
	}
};









int main() {
	WindowManager windowManager;
	Window window(1080, 800);

	SceneManager sceneManager;
	sceneManager.setActiveScene(std::make_shared<MainScene>());

	window.addEventListener(sceneManager.emitEvent);

	while (window.isOpen()) {
		sceneManager.update();

		windowManager.pollEvents();
	}
}
