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

		transform0.translation.z = 1;

		Renderer::Begin(Matrix4::CreatePerspective(2, 10, 10, 0.01, 10));

			transform0.rotation.y = currTP;
			transform0.rotation.z = currTP;

			Renderer::Clear(Vector4(0.1, 0.1, 0.1, 1.0));

			Renderer::RenderCube(transform0, wallTexture);

		Renderer::End();

		prevTP = currTP;

		App::EndFrame();
	}
}