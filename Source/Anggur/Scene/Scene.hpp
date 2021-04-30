#pragma once
#include <Anggur/Helper/Log.hpp>
#include <Anggur/Renderer/Renderer.hpp>

namespace Anggur {

enum class NodeType
{
    Default,
    Box,
    Rect
};

struct Node
{
    NodeType type;
    Transform transform;
    std::string name;
    std::vector<Node*> children;

    Node()
    {
        type = NodeType::Default;
        name = "Node";
    }

    virtual void OnUpdate() {}
    virtual void OnRender() {}
};

struct Box : public Node
{
    float rx;
    float ry;
    Color color;

    Box()
    {
        type   = NodeType::Box;
        name   = "Box";
        rx     = 0.25;
        ry     = 0.25;
        color  = Color::white;
    }

    void OnRender() override
    {
        Renderer::AddBox(Vector::zero, Vector(rx, ry), transform, color);
    }
};

class Scene
{
public:
    std::vector<Node*> tree;
    Node* selected;

    void OnAttach()
    {
        tree.push_back(new Box());
    }

    void OnUpdate()
    {
    }

    void OnDetach()
    {
    }
};

}
