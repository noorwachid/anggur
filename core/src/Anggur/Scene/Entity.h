#pragma once

#include "Anggur/Math/Matrix4.h"
#include "Transform.h"
#include <string>
#include <vector>
#include <memory>

namespace Anggur
{
    class Entity
    {
    public:
        std::vector<std::shared_ptr<Entity>> children;
        
    public:
        virtual void Initialize() {}
        virtual void Terminate() {}

        virtual void Update(float deltaTime) {}
    };    
}