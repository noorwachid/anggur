#include "Scene.hpp"

namespace Anggur {

void Scene::Restart()
{
    OnStart();
}

Scene::Scene()
{
    OnCreate();
}

Scene::~Scene()
{
    OnDestroy();
}

}
