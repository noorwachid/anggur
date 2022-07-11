#pragma once

#include "NodeManager.h"

namespace Anggur
{
    struct NodeManagerData 
    {
        std::shared_ptr<Node> rootNode;
    };

    NodeManagerData nodeManagerData;

    void NodeManager::SetRootNode(const std::shared_ptr<Node>& Node)
    {
        Terminate();

        nodeManagerData.rootNode = Node;

        Initialize();
    }

    const std::shared_ptr<Node>& NodeManager::GetRootNode()
    {
        return nodeManagerData.rootNode;
    }

    void NodeManager::Initialize()
    {
        if (nodeManagerData.rootNode) 
            nodeManagerData.rootNode->Initialize();
    }

    void NodeManager::Update(float deltaTime)
    {
        if (nodeManagerData.rootNode) 
        {
            nodeManagerData.rootNode->model = nodeManagerData.rootNode->transform.ToMatrix4();
            UpdateChildren(nodeManagerData.rootNode, deltaTime);
        }
    }

    void NodeManager::UpdateChildren(std::shared_ptr<Node>& node, float deltaTime)
    {
        for (auto child: node->children)
        {
            child->model = node->transform.ToMatrix4() * child->model;
            UpdateChildren(child, deltaTime);
        }
        node->Update(deltaTime);
    }

    void NodeManager::Terminate()
    {
        if (nodeManagerData.rootNode) 
            nodeManagerData.rootNode->Terminate();
    }
}