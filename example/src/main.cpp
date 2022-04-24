#include <Anggur/Core/Application.h>
#include <Anggur/Graphics/Function.h>
#include <Anggur/Graphics/Shader.h>
#include <Anggur/Graphics/VertexArray.h>
#include <Anggur/Graphics/Texture.h>
#include <Anggur/Utility/Log.h>
#include "Rx.h"
#include "RxState.h"

using namespace Anggur;

class Example3D: public Node {
public:
	void initialize() override {
		Rx::initialize();

		RxState::setClearColor(0.1f, 0.1f, 0.1f);

		ANGGUR_LOG("TEXTURE_SLOTS: %i", Texture::getMaxSlot());
	}

	void update() override {
		Rx::beginScene();

		RxState::clear();
		Rx::update();

		Rx::endScene();
	}
};

int main() {
	Application::initialize(Vector2(400, 400), "Anggur Example");
	Application::run(new Example3D());
}