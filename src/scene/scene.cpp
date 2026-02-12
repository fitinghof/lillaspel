#include "scene/scene.h"

Scene::Scene() : gameObjects(), finishedLoading(true) {
}

void Scene::SceneTick()
{
	for (size_t i = 0; i < this->gameObjects.size(); i++) {
		std::shared_ptr<GameObject> gameObject = this->gameObjects[i];
		gameObject->Tick();
		gameObject->LateTick();
	}

	this->DeleteDeleteQueue();

	ImGui::SetNextWindowSize(ImVec2(400.f, 500.f), ImGuiCond_FirstUseEver);
	ImGui::Begin("Object Hierarchy");
	if (ImGui::Button("Create")) ImGui::OpenPopup("select_gameobject");
	if (ImGui::BeginPopup("select_gameobject")) {
		if (ImGui::Selectable("GameObject")) 
		{
			this->CreateGameObjectOfType<GameObject>();
		}
			
		ImGui::EndPopup();
	}
	for (size_t i = 0; i < this->gameObjects.size(); i++) {
		std::weak_ptr<GameObject> gameObject = this->gameObjects[i];
		if (gameObject.lock()->parent.expired()) {
			ShowHierarchyRecursive("GameObject " + std::to_string(i), gameObject);
		}
	}
	ImGui::End();
}

void Scene::RegisterGameObject(std::shared_ptr<GameObject> gameObject)
{
	this->gameObjects.push_back(gameObject);
	gameObject->factory = this;
	gameObject->myPtr = gameObject;
	if (this->finishedLoading) {
		gameObject->Start();
	}
}

void Scene::QueueDeleteGameObject(std::weak_ptr<GameObject> gameObject)
{
	this->deleteQueue.push_back(gameObject);
}

size_t Scene::GetNumberOfGameObjects()
{
	return this->gameObjects.size();
}

void Scene::DeleteDeleteQueue()
{
	if (this->deleteQueue.empty()) {
		return;
	}

	Logger::Log("GameObject count ", this->gameObjects.size());

	if (this->gameObjects.empty()) {
		Logger::Warn("Tried to delete at least one GameObject, but there are no GameObjects in the scene. Aborted.");
		return;
	}

	for (size_t i = 0; i < this->deleteQueue.size(); i++) {
		if (this->deleteQueue[i].expired()) continue; // If object is already destroyed

		auto gameObject = this->deleteQueue[i].lock();

		auto iterator = std::find(this->gameObjects.begin(), this->gameObjects.end(), gameObject);

		if (iterator == this->gameObjects.end()) {
			Logger::Warn("Tried to delete a GameObject that is not present in the scene.");
			continue;
		}
		
		if (gameObject->GetChildCount() > 0) {
			for (auto child : gameObject->children) {
				this->deleteQueue.push_back(child);
			}
		}

		gameObject->OnDestroy();
		this->gameObjects.erase(iterator); // Actually deletes
	}

	this->deleteQueue.clear();

	Logger::Log("GameObject count ", this->gameObjects.size());
}

void Scene::CallStartOnAll() {
	for (size_t i = 0; i < this->gameObjects.size(); i++) {
		std::shared_ptr<GameObject> gameObject = this->gameObjects[i];
		gameObject->Start();
	}
}

void Scene::ShowHierarchyRecursive(std::string name, std::weak_ptr<GameObject> gameObject) 
{
	if (ImGui::TreeNode(name.c_str())) {
		gameObject.lock()->ShowInHierarchy();
		if (gameObject.lock()->children.size() > 0) {
			if (ImGui::TreeNode("Children")) {
				for (size_t i = 0; i < gameObject.lock()->children.size(); i++) {
					ShowHierarchyRecursive("GameObject " + std::to_string(i), gameObject.lock()->children[i].lock());
				}
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	};
}

const std::vector<std::shared_ptr<GameObject>>& Scene::GetGameObjects() const { return this->gameObjects; }
