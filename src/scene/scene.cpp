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

template<typename T>
inline GameObject* Scene::CreateGameObjectOfType(T*)
{	
	T* newObject = new T();
	if (GameObject* newGameObject = dynamic_cast<GameObject*>(newObject); newGameObject != nullptr)
	{
		this->gameObjects.push_back(std::unique_ptr<GameObject*>(newGameObject));

		return this->gameObjects[this->gameObjects.size() - 1].get();
	}

	throw std::runtime_error("Failed to add object, class not derived from GameObject.");
}
