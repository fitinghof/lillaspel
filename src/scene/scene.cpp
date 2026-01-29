#include "scene/scene.h"

Scene::Scene() : gameObjects()
{
}

void Scene::SceneTick()
{
	for (auto& gameObject : this->gameObjects) {
		gameObject->Tick();
	}
}
