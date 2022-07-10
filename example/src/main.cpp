#include "Anggur/Math/Matrix4.h"
#include "Anggur/System/Window.h"
#include "Anggur/System/Input.h"
#include "Anggur/System/Internal.h"
#include "VP2D.h"
#include "Renderer.h"
#include <iostream>
#include <chrono>

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

	struct Camera
	{
		Vector3 position;
		Vector3 target;
		Vector3 down;

		float fov = Math::DegreeToRadian(70.f);
	};
}

int main() 
{
	using namespace Anggur;

	App::Initialize(Vector2(400, 400));

	std::shared_ptr<Texture2D> wallTexture = std::make_shared<Texture2D>("./resources/images/f.png");

	Camera camera;
	camera.position.Set(0, 0, -10);

	float previousTime = 0;

	while (App::IsOpen()) 
	{
		App::BeginFrame();

		float currentTime = glfwGetTime();
		float deltaTime = currentTime - previousTime;

		previousTime = currentTime;

		Vector2 mousePosition = Input::GetMousePosition();
		Vector3 cameraDirection = Vector3::unitZ;
		float cameraMovementVelocity = 12.5;
		float cameraFovVelocity = 1.5;

		if (Input::IsKeyDown(Key::Left))
			camera.position.x -= cameraMovementVelocity * deltaTime;
		else if (Input::IsKeyDown(Key::Right))
			camera.position.x += cameraMovementVelocity * deltaTime;
		else if (Input::IsKeyDown(Key::Up))
			camera.position.z += cameraMovementVelocity * deltaTime;
		else if (Input::IsKeyDown(Key::Down))
			camera.position.z -= cameraMovementVelocity * deltaTime;

		float scrollDirectionY = Input::GetScrollDirection().y;
		std::cout << "SCROLL:" << scrollDirectionY << "\n";
		camera.fov += scrollDirectionY * cameraFovVelocity * deltaTime;

		camera.target = camera.position + cameraDirection;
		camera.down.Set(0, 1, 0);

		std::cout << camera.position.ToString() << "\n";

		Vector3 rotationAxis = Vector3::Normalize(Vector3(1.0, 1.0, 1.0));
		float rotationAngle = 0;

		Matrix4 model;
		model = model * Matrix4::CreateRotation(Quaternion(rotationAxis, rotationAngle));
		model = model * Matrix4::CreateTranslation(Vector3(0.0, 0.0, 0.0));

		Matrix4 view = Matrix4::CreateLookAt(camera.position, camera.target, camera.down);
		Matrix4 projection = Matrix4::CreatePerspective(camera.fov, 10, 10, 0.1, 100);

		Renderer::Begin(view * projection);

			Renderer::Clear(Vector4(0.1, 0.1, 0.1, 1.0));

			Renderer::RenderCube(Matrix4::CreateTranslation(Vector3::unitX + Vector3(0, 0, 10)), wallTexture, Vector4::red);
			Renderer::RenderCube(Matrix4::CreateTranslation(Vector3::unitY + Vector3(0, 0, 10)), wallTexture, Vector4::blue);
			Renderer::RenderCube(Matrix4::CreateTranslation(Vector3::unitZ + Vector3(0, 0, 10)), wallTexture, Vector4::green);

		Renderer::End();

		std::cout << Vector3::Cross(Vector3::unitX, Vector3::unitY).ToString() << "\n";

		App::EndFrame();
	}
}