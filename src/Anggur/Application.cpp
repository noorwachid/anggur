#include "System/Input.h"
#include "System/Internal.h"
#include "Graphics/Renderer.h"
#include "Scene/NodeManager.h"
#include "Application.h"
#include <iostream>
#include <chrono>

namespace Anggur
{
	struct ApplicationData 
	{
		float previousTime = 0.0f;
		float currentTime = 0.0f;
	};

	ApplicationData applicationData;

	void Application::Initialize(const Vector2& windowSize)
    {
        Window::Initialize(windowSize);
        Input::Initialize();
        
        Renderer::Initialize();
    }

    void Application::Terminate()
    {
    }

    bool Application::IsOpen()
    {
        return Window::IsOpen();
    }

    void Application::BeginFrame() 
    {
        Window::PollEvents();
        
        applicationData.currentTime = glfwGetTime();
    }

    void Application::EndFrame()
    {
        Window::SwapBuffers();
        Input::Update();

        applicationData.previousTime = applicationData.currentTime;
    }
	
    void Application::Run()
    {
        while (Application::IsOpen()) 
        {
            Application::BeginFrame();

            NodeManager::Update(applicationData.currentTime - applicationData.previousTime);

            Application::EndFrame();
        }
    }
}