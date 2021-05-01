# Anggur: 2D Rendering Library

> {NOTE} 
> Do not use for serious project unless you know what are you doing.

## Brief Preview
``` cxx
#include <Anggur/System/Application.hpp>
#include <Anggur/Helper/Log.hpp>

using namespace Anggur;
using R = Renderer;

class App: public Application
{
    Camera camera;

    void OnConfigure(WindowConfig& config) override
    {
        config.width = 1000;
        config.height = 500;

        camera.SetRatio((float) config.width / (float) config.height);
        camera.Scale(Vector(0.05, 0.05));
        camera.Translate(Vector(0, 0.7));
    }

    void OnUpdate(float dx) override
    {
        R::ClearBackground();
        R::BeginScene(camera);
        
        R::AddBox(Vector(0, 0), 1);

        R::EndScene();
    }
};

int main() 
{
	App app;
	app.Run();
}


```

## Contribute
### Coding Style
``` cxx
#define Namespace_ThingYouWantTodo 0

int gMagicNumber = 8;

// use verb like word for functions
int AddTwoNumber(int a, int b);

// use noun for variables and classes
// use camel case for variables and parameters
// anything else should be in pascal case

int numberOne;

class Player 
{
public:
    Vector GetPos() 
    {
        return pos;
    }

private:
    Vector pos;
};

enum class PlayerState
{
   Idle,
   Run,
   Jump,
};

```
