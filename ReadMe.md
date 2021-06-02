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

    void OnAttach() override
    {
        camera.viewport = mWindow->GetSize();
        camera.distance = 0.0125;
        camera.offset.Set(0.0, -10);
    }

    void OnUpdate(float dx) override
    {        
        R::ClearBackground();
        R::BeginScene(camera);

        R::AddCircle(Vector(0, 12), 10, Color::Purple);
        R::AddCircle(Vector(-12, -9), 10, Color::Purple);
        R::AddCircle(Vector(12, -9), 10, Color::Purple);

        R::AddPolyring({Vector(0, 40), Vector(-35, -22), Vector(35, -22)}, 1);

        R::EndScene();
    }
};

Anggur_EntryPoint(App);
```

## Contribute
### Coding Style
I use Unity coding convention

``` cxx
#define Namespace_ThingYouWantTodo 0

int magicNumber = 8;

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
