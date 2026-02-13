#include "scene/scene.h"
#include "core/imguiManager.h"

Scene::Scene() : gameObjects(), finishedLoading(true), currentGameObjectId(0) {
}

void Scene::SceneTick(bool isPaused)
{
	for (size_t i = 0; i < this->gameObjects.size(); i++) {
		std::shared_ptr<GameObject> gameObject = this->gameObjects[i];

		if (!gameObject->IsActive()) continue;

		// If paused, only debug camera should run
		if (isPaused) {
			if (DebugCamera* cam = dynamic_cast<DebugCamera*>(gameObject.get()); cam == nullptr) {
				continue;
			}
		}

		gameObject->Tick();
		gameObject->LateTick();
	}

	this->DeleteDeleteQueue();

	ShowHierarchy();
}

void Scene::RegisterGameObject(std::shared_ptr<GameObject> gameObject)
{
	this->gameObjects.push_back(gameObject);
	gameObject->factory = this;
	gameObject->myPtr = gameObject;
	gameObject->SetName("GameObject " + std::to_string(this->GetNextID()));
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
	return this->gameObjects.size(); }

int Scene::GetNextID() 
{ 
	return this->currentGameObjectId++; 
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

void Scene::ShowHierarchy() 
{
	if (!ImguiManager::showObjectHierarchy) return;

	ImGui::SetNextWindowSize(ImVec2(400.f, 500.f), ImGuiCond_FirstUseEver);
	ImGui::Begin("Object Hierarchy", &ImguiManager::showObjectHierarchy, ImGuiWindowFlags_MenuBar);
	if (ImGui::Button("Create")) ImGui::OpenPopup("select_gameobject");
	if (ImGui::BeginPopup("select_gameobject")) {
		if (ImGui::Selectable("GameObject")) {
			this->CreateGameObjectOfType<GameObject>();
		}
		if (ImGui::Selectable("GameObject3D")) {
			this->CreateGameObjectOfType<GameObject3D>();
		}
		if (ImGui::Selectable("MeshObject")) {
			this->CreateGameObjectOfType<MeshObject>();
		}
		if (ImGui::Selectable("CameraObject")) {
			this->CreateGameObjectOfType<CameraObject>();
		}
		 //Debug Camera apparently breaks
		if (ImGui::Selectable("DebugCamera")) {
			this->CreateGameObjectOfType<DebugCamera>();
		}
		if (ImGui::Selectable("SpotlightObject")) {
			this->CreateGameObjectOfType<SpotlightObject>();
		}

		ImGui::EndPopup();
	}
	for (size_t i = 0; i < this->gameObjects.size(); i++) {
		std::weak_ptr<GameObject> gameObject = this->gameObjects[i];
		if (gameObject.lock()->parent.expired()) {
			ShowHierarchyRecursive(gameObject.lock()->name, gameObject);
		}
	}
	ImGui::End();
}

void Scene::ShowHierarchyRecursive(std::string name, std::weak_ptr<GameObject> gameObject) {
	if (ImGui::TreeNode(name.c_str())) {
		gameObject.lock()->ShowInHierarchy();
		ImGui::Separator();
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
