#include "Anggur/Math/Matrix4.h"
#include "Anggur/System/Window.h"
#include "Anggur/System/Input.h"
#include "Anggur/System/Internal.h"
#include "VP2D.h"
#include "Renderer.h"
#include <iostream>

namespace Anggur
{
	class App 
	{
	public:
		static void Initialize(const Vector2& windowSize)
		{
			Window::Initialize(windowSize);
			Input::Initialize();

			Renderer::Initialize();
		}

		static bool IsOpen()
		{
			return Window::IsOpen();
		}

		static void BeginFrame() 
		{
			Window::PollEvents();
		}

		static void EndFrame()
		{
			Window::SwapBuffers();
			Input::Update();
		}
	};

	 Matrix4 CreatePerspective(float fovY, float width, float height, float near, float far)
	{
		float yScale = Math::Cot(fovY / 2.0f);
		float xScale = yScale * height / width;
		
		return Matrix4(
			xScale, 0.0f, 0.0f, 0.0f,
			0.0f, yScale, 0.0f, 0.0f,
			0.0f, 0.0f, far / (far - near), 1.0f,
			0.0f, 0.0f, -near * far / (far - near), 0.0
		);
	}

	Matrix4 CreateOrtho(float width, float height, float near, float far)
	{
		return Matrix4(
			2.0f / width, 0.0f, 0.0f, 0.0f,
			0.0f, 2.0f / height, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f / (far - near), 0.0f,
			0.0f, 0.0f, near / (near - far), 1.0
		);
	}
}

int main() 
{
	using namespace Anggur;

	App::Initialize(Vector2(400, 400));

	std::shared_ptr<Texture2D> wallTexture = std::make_shared<Texture2D>("./resources/images/f.png");

	Transform transform0;

	transform0.translation.z = 1;

	double prevTP = glfwGetTime();

	while (App::IsOpen()) 
	{
		App::BeginFrame();

		double currTP = glfwGetTime();
		double delta = currTP - prevTP;

		// Renderer::Begin(Matrix4(
		// 	1.0f, 0.0f, 0.0f, 0.0f,
		// 	0.0f, 1.0f, 0.0f, 0.0f,
		// 	0.0f, 0.0f, 1.0f, 0.0f,
		// 	0.0f, 0.0f, 0.0f, 1.0f
		// ));

		transform0.translation.z = 2;

		// Renderer::Begin(CreateOrtho(4, 4, -2, 2));
		Renderer::Begin(CreatePerspective(2, 10, 10, 0.01, 10));

			transform0.rotation.y = currTP;
			transform0.rotation.z = currTP;

			Renderer::Clear(Vector4(0.1, 0.1, 0.1, 1.0));

			Renderer::RenderCube(transform0);

		Renderer::End();

		prevTP = currTP;

		App::EndFrame();
	}
}