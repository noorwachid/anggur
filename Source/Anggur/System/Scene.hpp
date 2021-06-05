#pragma once
#include <Anggur/Helper/Log.hpp>
#include <Anggur/Renderer/Renderer.hpp>
#include "Event/Event.hpp"

namespace Anggur {

class Scene
{
public:
    void Reattach();

    virtual void OnAttach() {}
    virtual void OnUpdate() {}
    virtual void OnDetach() {}
    virtual void OnEvent(Event& event) {}

    virtual ~Scene() {}
};

}
