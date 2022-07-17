#include "Anggur/IO/WindowSystem.h"
#include "Anggur/IO/Window.h"

int main() 
{
	using namespace Anggur;

	WindowSystem windowSystem;
	Window window(Vector2(600, 400), "A");

	while (window.IsOpen())
	{
		windowSystem;
	}
}