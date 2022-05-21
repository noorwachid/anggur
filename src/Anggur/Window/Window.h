#pragma once

#include <string>
#include <vector>
#include <functional>
#include <Anggur/Window/Event/Common.h>
#include <Anggur/Math/Vector2.h>

struct GLFWwindow;

namespace Anggur 
{
	using WindowHandler = GLFWwindow;
	using EventListener = std::function<void (Event&)>;

	class Window {
		// TODO: Implement subwindow

	public:
		Window(const Vector2& size = Vector2(800, 600), const std::string& title = "");

		~Window();

		void SetPosition(const Vector2& pos);
		void SetSize(const Vector2& size);
		void SetTitle(const std::string& title);

		float GetAspectRatio();
		const Vector2& GetPosition();
		const Vector2& GetSize();
		const std::string& GetTitle();
		WindowHandler* GetHandler();

		bool IsOpen();
		void Close();
		void Bind();
		
		void SwapBuffers();

		void AddEventListener(EventListener eventListener);
		void RemoveEventListener(EventListener eventListener);

	private:
		void InitializeGraphicsFunctions();
		void InitializeEventEmmiter();
		void EmitEvent(Event& event);

	private:
		WindowHandler* handler = nullptr;
		std::vector<EventListener> eventListeners;
		std::string title;

		Vector2 size;
		Vector2 position;
	};
}
