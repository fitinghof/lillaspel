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

private:
	std::vector<std::shared_ptr<GameObject>> gameObjects;
};

template<typename T>
inline std::weak_ptr<T> Scene::CreateGameObjectOfType()
{
	// Make sure it is a gameObject (compiler assert)
	Logger::Warn(__LINE__);
	static_assert(std::is_base_of_v<GameObject, T>, "T must derive from GameObject");
	Logger::Warn(__LINE__);

	auto obj = std::make_shared<T>();
	Logger::Warn(__LINE__);
	std::weak_ptr<T> newObject = obj;
	Logger::Warn(__LINE__);
	obj->SetWeakPtr(newObject);
	Logger::Warn(__LINE__);
	this->gameObjects.push_back(std::move(obj));
	Logger::Warn(__LINE__);
	return newObject;
}
