#include "Anggur/Application.h"
#include "Anggur/Audio/AudioBuffer.h"
#include "Anggur/Audio/AudioSource.h"
#include "Anggur/Graphics/Renderer.h"
#include "Anggur/OS/Window.h"
#include "GLFW/glfw3.h"

#include <fstream>
#include <iostream>

using namespace Anggur;

class TestScene : public Scene
{
public:
	virtual void Initialize() override
	{
		_renderer->SetView(
			Matrix3::CreatePixelPerfect(Vector2(-1, -1), Vector2(_window->GetWidth(), _window->GetHeight()))
		);

		_buffer = new AudioBuffer("resource/sample.wav");

		_source1 = new AudioSource();
		_source1->SetBuffer(_buffer);

		_source2 = new AudioSource();
		_source2->SetBuffer(_buffer);
		_source2->SetPitch(1.2f);

		_source3 = new AudioSource();
		_source3->SetBuffer(_buffer);
		_source3->SetPitch(1.4f);

		_source4 = new AudioSource();
		_source4->SetBuffer(_buffer);
		_source4->SetPitch(1.6f);
	}

	virtual void Terminate() override
	{

		delete _source1;
		delete _source2;
		delete _source3;
		delete _source4;
		delete _buffer;
	}

	virtual void Update(float deltaTime) override
	{
		_renderer->BeginScene();

			if (glfwGetKey(_window->GetContext(), GLFW_KEY_A) == GLFW_PRESS)
				_source1->Play();

			if (glfwGetKey(_window->GetContext(), GLFW_KEY_S) == GLFW_PRESS)
				_source2->Play();

			if (glfwGetKey(_window->GetContext(), GLFW_KEY_D) == GLFW_PRESS)
				_source3->Play();

			if (glfwGetKey(_window->GetContext(), GLFW_KEY_F) == GLFW_PRESS)
				_source4->Play();


		_renderer->ClearBackground(Vector4(0.1, 0.1, 0.1, 1.0));

		_renderer->EndScene();
	}

private:
	float _x;

	AudioBuffer* _buffer;
	AudioSource* _source1;
	AudioSource* _source2;
	AudioSource* _source3;
	AudioSource* _source4;

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
