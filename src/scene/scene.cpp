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

void Scene::RegisterGameObject(std::shared_ptr<GameObject> gameObject)
{
	this->gameObjects.push_back(gameObject);
	gameObject->factory = this;
	gameObject->Start();
}

void Scene::QueueDeleteGameObject(std::weak_ptr<GameObject> gameObject)
{
	deleteQueue.push_back(gameObject.lock());
}

void Scene::DeleteDeleteQueue()
{
	for(auto& gameObject : de)
}
