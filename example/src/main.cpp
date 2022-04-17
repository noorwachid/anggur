#include "Anggur/Core/Application.h"
#include "Anggur/Graphic/Function.h"
#include "Anggur/Graphic/Renderer.h"

using namespace Anggur;

class Example3D: public Node {
public:
	void initialize() override {

	}

	void update() override {
		Renderer::clear(Vector4::charcoal);
	}
};

int main() {
	Application::initialize(Vector2(400, 400), "Anggur Example");
	Application::run(new Example3D());
}