#pragma once

#include <Anggur/Scene/Scene.h>
#include <memory>

namespace Anggur {
    class SceneManager {
    public:
        static void set(const std::shared_ptr<Scene>& scene);

        static const std::shared_ptr<Scene>& get();
    };
}