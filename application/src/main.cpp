#include "Anggur/Math/Matrix4.h"
#include "Anggur/System/Window.h"
#include "Anggur/System/Input.h"
#include "Anggur/System/Internal.h"
#include "Anggur/Graphics/Renderer.h"
#include "Anggur/Scene/EntityHierarchy.h"
#include "Anggur/Application.h"
#include "Scenes/Scene2D.h"

int main() 
{
	using namespace Anggur;

	Application::Initialize(Vector2(400, 400), "Anggur.Example");

	EntityHierarchy::SetRootEntity(std::make_shared<Scene2D>());

	Application::Run();
	Application::Terminate();
}