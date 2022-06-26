#include "Anggur/Math/Matrix4.h"
#include "Anggur/System/Window.h"
#include "Anggur/System/Input.h"
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

	Matrix4 CreatePerspectiveFOV(float fovY, float width, float height, float near, float far)
	{
		float yScale = Math::Cot(fovY / 2.0f);
		float xScale = yScale * height / width;
		
		return Matrix4({
			xScale, 0.0f, 0.0f, 0.0f,
			0.0f, yScale, 0.0f, 0.0f,
			0.0f, 0.0f, far / (far - near), 1.0f,
			0.0f, 0.0f, -near * far / (far - near), 0.0,
		});
	}

	Matrix4 CreateOrtho(float width, float height, float near, float far)
	{
		return Matrix4({
			2.0f / width, 0.0f, 0.0f, 0.0f,
			0.0f, 2.0f / height, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f / (far - near), 0.0f,
			0.0f, 0.0f, near / (near - far), 1.0f,
		});
	}
}

int main() 
{
	using namespace Anggur;

	App::Initialize(Vector2(400, 400));

	std::shared_ptr<Texture2D> wallTexture = std::make_shared<Texture2D>("./resources/images/f.png");

	Transform transform;
	transform.rotation.z = Math::pi / 4;
	transform.translation.z = -1;

	while (App::IsOpen()) 
	{
		App::BeginFrame();

		// Renderer::Begin(CreatePerspectiveFOV(Math::pi / 9, 4.0f, 4.0f, -10.0f, 10.f));

		Renderer::Begin(CreateOrtho(5.0f, 5.0f, 0.0f, 100.0f));

			Renderer::Clear();
			
			Renderer::RenderRectangle(transform, wallTexture, Vector2::zero, Vector2::one);

		Renderer::End();

		App::EndFrame();
	}
}