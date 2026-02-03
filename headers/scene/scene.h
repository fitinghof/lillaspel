#pragma once

#include <vector>
#include "gameObjects/gameObject.h"
#include "utilities/logger.h"
#include <memory>

class Scene {
public:
	Scene();
	~Scene() = default;

	void SceneTick();

	template <typename T>
	inline std::weak_ptr<T> CreateGameObjectOfType();

	void Test() {
		Logger::Log("Works");
	}

private:
	std::vector<std::shared_ptr<GameObject>> gameObjects;
};

template<typename T>
inline std::weak_ptr<T> Scene::CreateGameObjectOfType()
{
	// Make sure it is a gameObject (compiler assert)
	static_assert(std::is_base_of_v<GameObject, T>, "T must derive from GameObject");

	auto obj = std::make_shared<T>();
	std::weak_ptr<T> newObject = obj;
	obj->SetWeakPtr(newObject);
	obj->scene = this;
	obj->Start();

	this->gameObjects.push_back(std::move(obj));

	return newObject;
}
