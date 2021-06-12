#pragma once
#include <Anggur/Helper/Log.hpp>
#include <Anggur/Renderer/Renderer.hpp>
#include "Event/Event.hpp"

namespace Anggur {

class Scene
{
public:
    void Restart();

    virtual void Create() {}
    virtual void Start() {}
    virtual void Update() {}
    virtual void Destroy() {}

    virtual void OnEvent(Event& event) {}

    virtual ~Scene() {}
};

}
