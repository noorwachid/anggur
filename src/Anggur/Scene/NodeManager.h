#pragma once

#include "Node.h"
#include <memory>

namespace Anggur
{
    class NodeManager
    {
    public:
        static void SetRootNode(const std::shared_ptr<Node>& node);
        static const std::shared_ptr<Node>& GetRootNode();

        static void Initialize();
        static void Update(float deltaTime);
        static void UpdateChildren(std::shared_ptr<Node>& node, float deltaTime);

        static void Terminate();
    };    
}