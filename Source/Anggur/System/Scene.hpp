#pragma once
#include <Anggur/Helper/Log.hpp>
#include <Anggur/Renderer/Renderer.hpp>
#include "Event/Event.hpp"

namespace Anggur {

class Scene
{
public:
    void Restart();

    virtual void OnCreate() {}
    virtual void OnStart() {}
    virtual void OnUpdate() {}
    virtual void OnDestroy() {}
    virtual void OnEvent(Event& event) {}

    Scene();
    virtual ~Scene();
};

}
