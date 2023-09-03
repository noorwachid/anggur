#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Anggur/OS/Window.h"
#include "Anggur/Graphics/Renderer.h"

#include <iostream>
#include <fstream>

using namespace Anggur;

class Input
{
public:
	Input(Window& window): _window(window)
	{
	}

	bool IsKeyPressed(int key) const
	{
		return glfwGetKey(_window.GetContext(), key) == GLFW_PRESS;
	}

private:
	Window& _window;
};

int main()
{
	Window window(600, 400);

	window.Connect([](void* process) {
		return gladLoadGLLoader(reinterpret_cast<GLADloadproc>(process));
	});

	Input input(window);

	Renderer renderer;

    while (!window.ShouldClose())
    {
        if (input.IsKeyPressed(GLFW_KEY_ESCAPE))
		{
			window.Close();
			continue;
		}

		renderer.BeginScene();

		renderer.ClearBackground(Vector4(0.1, 0.1, 0.1, 1.0));

		renderer.DrawCircle(Vector2(0, 0), 0.5, 0.25, Math::epsilon, Vector4::white);

		renderer.EndScene();
 
        window.SwapBuffers();
        window.PollEvents();
    }

    return 0;
}

