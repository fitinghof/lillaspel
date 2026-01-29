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
	GameObject* CreateGameObjectOfType(T*);

private:
	std::vector<std::unique_ptr<GameObject>> gameObjects;
};
