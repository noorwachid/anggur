#pragma once

#include "Anggur/Math/Matrix4.h"
#include "Transform.h"
#include <string>
#include <vector>
#include <memory>

namespace Anggur
{
    class Node
    {

    public:
        virtual void Initialize() {}
        virtual void Terminate() {}
        
        virtual void Update(float deltaTime) {}
        virtual void FixedUpdate(float deltaTime) {}
    };    
}