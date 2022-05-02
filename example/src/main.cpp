#include <Anggur/Core/Application.h>
#include <Anggur/Graphics/Function.h>
#include <Anggur/Graphics/Shader.h>
#include <Anggur/Graphics/VertexArray.h>
#include <Anggur/Graphics/Texture.h>
#include <Anggur/Utility/Log.h>
#include <Anggur/Math/Random.h>
#include "Renderer.h"

using namespace Anggur;

class RootNode: public Node {
private:
	struct Rectangle {
		Vector2 position;
		Vector2 size;
		Vector4 color;
	};

	std::vector<Rectangle> rectangles;

public:
	void initialize() override {
		Renderer::initialize();
		Renderer::setClearColor(Vector4::charcoal);

		Vector2 windowSize = Application::getWindow()->getSize();

		for (size_t i = 0; i < 100; ++i) {
			Rectangle rectangle;
			rectangle.position.set(Random::range(0, windowSize.x), Random::range(0, windowSize.y));
			rectangle.size.set(Random::range(5, 50), Random::range(5, 50));
			rectangle.color.set(Random::getNormal(), Random::getNormal(), Random::getNormal());

			rectangles.push_back(std::move(rectangle));
		}
	}

	Matrix3 createScreenVP(float scale = 1.0f) {
		Vector2 windowSize = Application::getWindow()->getSize();

		return Matrix3({
			2.0f / windowSize.x * scale, 0.0f, 0.0f,
			0.0f, -2.0f / windowSize.y * scale, 0.0f,
			-1.0f, 1.0f, 1.0f,
		});
	} 

	void render() override {
		Renderer::beginScene();

		Renderer::clear();

		Renderer::setViewProjection(createScreenVP());

		// Renderer::renderRectangle(Vector2::zero + 10, Vector2(Application::getWindow()->getSize()) - 20, Vector4::white);
		// Renderer::renderRectangle(Vector2::zero + 20, Vector2(Application::getWindow()->getSize()) - 40, Vector4::red);

		Renderer::renderRectangle(Vector2(20, 20), Vector2(10, 10));
		Renderer::renderRectangle(Vector2(40, 20), Vector2(10, 10));

		// for (Rectangle& rectangle: rectangles) {
	    // 	Renderer::renderRectangle(rectangle.position, rectangle.size, rectangle.color);
		// }

		Renderer::endScene();
	}
};

int main() {
	Application::initialize(Vector2(600, 400), "Anggur Example");
	Application::run(new RootNode());
}