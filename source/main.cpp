#include "Anggur/Application.h"
#include "Anggur/Graphics/Renderer.h"
#include "Anggur/OS/Window.h"

#include <fstream>
#include <iostream>

using namespace Anggur;

class TestScene : public Scene
{
	virtual void Initialize() override
	{
		_renderer->SetView(
			Matrix3::CreatePixelPerfect(Vector2(-1, -1), Vector2(_window->GetWidth(), _window->GetHeight()))
		);
	}

	virtual void Update(float deltaTime) override
	{
		_renderer->BeginScene();

		_renderer->ClearBackground(Vector4(0.1, 0.1, 0.8, 1.0));
		_renderer->DrawCircle(Vector2(0, 0), 100, 10, Math::epsilon, Vector4::white);

		_renderer->EndScene();
	}
};

int main()
{
	try 
	{
		Application application(400, 300);
		application.SetScene<TestScene>();
		application.Run();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << "\n";
	}

	return 0;
}
