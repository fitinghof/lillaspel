#pragma once

#include <vector>
#include "gameObjects/gameObject.h"
#include "utilities/logger.h"

class Scene {
public:
	Scene();
	~Scene() = default;

	void SceneTick();

	template <typename T>
	GameObject* CreateGameObjectOfType();

private:
	std::vector<std::unique_ptr<GameObject>> gameObjects;
};

template<typename T>
inline GameObject* Scene::CreateGameObjectOfType()
{
	static_assert(std::is_base_of_v<GameObject, T>,
		"T must derive from GameObject");

	auto obj = std::make_unique<T>();
	T* newObject = obj.get();
	this->gameObjects.push_back(std::move(obj));
	return newObject;
}
