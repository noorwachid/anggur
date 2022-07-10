#pragma once

namespace Anggur
{
    class Node
    {
    public:
        virtual void Initialize() {}
        virtual void Update(float deltaTime) {}
        virtual void Terminate() {}
    };    
}