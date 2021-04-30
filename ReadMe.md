# Anggur: 2D C++ Game Engine

Another day of new C++ game engine.

> {NOTE} 
> Do not use for serious project unless you know what are you doing.

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
