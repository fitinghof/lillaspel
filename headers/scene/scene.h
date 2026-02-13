#pragma once

#include "gameObjects/gameObject.h"
#include "gameObjects/gameObjectFactory.h"
#include "utilities/logger.h"
#include <algorithm>
#include <memory>
#include <vector>

#include "gameObjects/debugCamera.h"
#include "gameObjects/meshObject.h"
#include "gameObjects/spotlightObject.h"

class SceneManager;

class Scene : public GameObjectFactory {
public:
	Scene();
	~Scene() = default;

	void SceneTick(bool isPaused);

	virtual void RegisterGameObject(std::shared_ptr<GameObject> gameObject) override;

	virtual void QueueDeleteGameObject(std::weak_ptr<GameObject> gameObject) override;

	size_t GetNumberOfGameObjects();

	virtual int GetNextID() override;

private:
	/// <summary>
	/// Deletes all GameObjects in the deleteQueue (that is, all objects added using QueueDeleteGameObject())
	/// </summary>
	void DeleteDeleteQueue();

	/// <summary>
	/// Used after finishing loading a scene
	/// </summary>
	void CallStartOnAll();

	void ShowHierarchy();
	void ShowHierarchyRecursive(std::string name, std::weak_ptr<GameObject> gameObject);

	std::vector<std::shared_ptr<GameObject>> gameObjects;
	std::vector<std::weak_ptr<GameObject>> deleteQueue;

	friend SceneManager;

	virtual const std::vector<std::shared_ptr<GameObject>>& GetGameObjects() const override;

	bool finishedLoading;

	int currentGameObjectId;
};
