#pragma once

#include <vector>
#include "gameObjects/gameObject.h"
#include "utilities/logger.h"
#include <memory>
#include "gameObjects/gameObjectFactory.h"
#include <algorithm>

class Scene : public GameObjectFactory {
public:
	Scene();
	~Scene() = default;

	void SceneTick();

	virtual void RegisterGameObject(std::shared_ptr<GameObject> gameObject) override;
	virtual void QueueDeleteGameObject(std::weak_ptr<GameObject> gameObject) override;

	size_t GetNumberOfGameObjects();

private:
	void DeleteDeleteQueue();

	std::vector<std::shared_ptr<GameObject>> gameObjects;
	std::vector<std::weak_ptr<GameObject>> deleteQueue;
};
