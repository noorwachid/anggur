#pragma once

#include "Event/Event.hpp"

namespace Anggur {

class Scene
{
public:
    virtual void Create() {}
    virtual void Start() {}
    virtual void Update() {}
    virtual void Stop() {}
    virtual void Destroy() {}

    virtual void OnEvent(Event& event) {}

    virtual ~Scene() {}
};

}
