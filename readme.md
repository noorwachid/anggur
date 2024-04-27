# Anggur

> [!WARNING]
> Do not use for serious project unless you know what are you doing.

```cpp
// main.cpp

#include "Anggur/Application.h"

using namespace Anggur;

class TestScene : public Scene 
{
public:
	virtual void Initialize() override
	{
		_renderer->SetView(
			Matrix3::CreatePixelPerfect(
				Vector2(0, 0), 
				Vector2(_window->GetWidth(), _window->GetHeight())
			)
		);
	}

	virtual void Update(float deltaTime) override
	{
		_renderer->BeginScene();
		_renderer->Clear(Vector4::charcoal);
		_renderer->DrawCircle(Vector2(0, 0), 10, 10, 0.01, Vector4::purple);
		_renderer->EndScene();
	}

private:
	float _elapsedTime;
};

int main() 
{
	Application application(400, 300, "sandbox");
	application.SetScene(new TestScene);
	application.Run();

	return 0;
}
```
