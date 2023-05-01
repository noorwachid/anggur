#pragma once 

#include "Anggur/Graphics/Render/Renderer.h"
#include "Anggur/Common/Integral.h"
#include "Anggur/IO/Window.h"
#include "Anggur/Math/Vector2.h"
#include "Anggur/Scene/Node.h"
#include "Anggur/Scene/Scene.h"
#include <string>

namespace Anggur 
{
	class NodeUI : public Node
	{
	public:
		Vector2 position;
		Vector2 size;
		Vector4 backgroundColor{1, 1, 1, 1};

	public:
		void Draw() override
		{
			renderer->BeginScene(Matrix3::CreatePixelPerfect(Vector2(0, 0), window->GetSize()));
		}

		void LateDraw() override
		{
			renderer->EndScene();
		}
	};

	class View : public NodeUI
	{
	public:
		void Draw() override
		{
			Matrix3 model(1, 0, 0, 0, 1, 0, position.x, position.y, 1);
			renderer->DrawRectangle(model, Vector2::zero, size, backgroundColor);
		}
	};

	class Text : public NodeUI
	{
	public:
		Font* font = nullptr;

		void SetFont(const std::string& name)
		{
			font = scene->fontSystem.GetFont(name);
		}

		void Draw() override 
		{
			Matrix3 model(1, 0, 0, 0, 1, 0, position.x, position.y, 1);
			renderer->DrawText(model, Vector2::zero, "HELLO", font, 18);
		}
	};

	class Scroll : public NodeUI
	{
	public:
		void Draw() override
		{
			Matrix3 modelBackground(1, 0, 0, 0, 1, 0, position.x, position.y, 1);
			renderer->DrawRectangle(modelBackground, Vector2::zero, size, backgroundColor);

			renderer->BeginMask();

			renderer->BeginWriteMask();
				Matrix3 model(1, 0, 0, 0, 1, 0, position.x, position.y, 1);
				renderer->DrawRectangle(model, Vector2::zero, size, backgroundColor);
			renderer->EndWriteMask();
		}

		void LateDraw() override
		{
			renderer->BeginEraseMask();
				Matrix3 model(1, 0, 0, 0, 1, 0, position.x, position.y, 1);
				renderer->DrawRectangle(model, Vector2::zero, size, backgroundColor);
			renderer->EndEraseMask();

			renderer->EndMask();
		}
	};
}
