#include "Anggur/Math/Matrix4.h"
#include "Anggur/System/Window.h"
#include "Anggur/System/Input.h"
#include "Anggur/System/Internal.h"
#include "Anggur/Graphics/Renderer.h"
#include "Anggur/Scene/SceneManager.h"
#include "Anggur/Application.h"
#include "Scenes/ExampleScene2D.h"

int main() 
{
	using namespace Anggur;

	Application::Initialize(Vector2(400, 400));

	SceneManager::SetActiveScene(std::make_shared<ExampleScene2D>());

	Application::Run();
	Application::Terminate();
}