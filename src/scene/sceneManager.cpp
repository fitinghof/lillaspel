#include "scene/sceneManager.h"

// Very good macro, please don't remove
#define NAMEOF(x) #x

SceneManager::SceneManager(Renderer *rend) : mainScene(nullptr), renderer(rend), objectFromString()
{
	this->objectFromString.RegisterType<GameObject>(NAMEOF(GameObject));
	this->objectFromString.RegisterType<GameObject3D>(NAMEOF(GameObject3D));
	this->objectFromString.RegisterType<MeshObject>(NAMEOF(MeshObject));
	this->objectFromString.RegisterType<SpotlightObject>(NAMEOF(SpotlightObject));
	this->objectFromString.RegisterType<CameraObject>(NAMEOF(CameraObject));
	this->objectFromString.RegisterType<DebugCamera>(NAMEOF(DebugCamera));

	CreateNewScene(this->emptyScene);
	this->emptyScene->CreateGameObjectOfType<CameraObject>();
}

void SceneManager::SceneTick()
{
	if (!this->mainScene.get()) {
		this->mainScene = this->emptyScene;
	}

	this->mainScene->SceneTick();

	ImGui::Begin("SceneTest");
	if (ImGui::Button("Delete Scene")) {
		DeleteScene(this->mainScene);
	}
	ImGui::End();
}

void SceneManager::LoadScene(Scenes scene)
{
	switch (scene) {
	case Scenes::EMPTY:
		break;
	case Scenes::MAIN_MENU:
		Logger::Warn("There is no main menu scene.");
		break;
	case Scenes::GAME:
		Logger::Warn("There is no game scene.");
		break;
	case Scenes::END_CREDITS:
		Logger::Warn("There is no end credits scene.");
		break;
	case Scenes::DEMO:
		LoadSceneFromFile("../../assets/scenes/testresult.json");
		break;
	default:
		break;
	}
}

void SceneManager::CreateNewScene(std::shared_ptr<Scene>& scene)
{
	if (scene.get())
	{
		DeleteScene(scene);
	}

	scene = std::make_shared<Scene>();
}

void SceneManager::DeleteScene(std::shared_ptr<Scene>& scene)
{
	scene.reset();
	RenderQueue::ClearAllQueues();
	Logger::Log("Deleted scene: ", this->currentScenePath);
}

void SceneManager::LoadSceneFromFile(const std::string &filePath)
{
	CreateNewScene(this->mainScene);

	std::ifstream file(filePath);
	nlohmann::json data = nlohmann::json::parse(file);
	file.close();
	this->currentScenePath = filePath;

	// Actual loading
	CreateObjectsFromJsonRecursively(data["gameObjects"], std::shared_ptr<GameObject>(nullptr));

	SetMainCameraInScene(this->mainScene);
}

void SceneManager::CreateObjectsFromJsonRecursively(const nlohmann::json &data, std::weak_ptr<GameObject> parent)
{
	for (const nlohmann::json &objectData : data)
	{
		// Logger::Log(objectData.dump());

		if (!objectData.contains("type"))
		{
			throw std::runtime_error("Failed to load scene: GameObject doesn't have a type.");
		}

		GameObject *gameObjectPointer = static_cast<GameObject *>(objectFromString.Construct(objectData.at("type")));
		auto obj = std::shared_ptr<GameObject>(gameObjectPointer);
		this->mainScene->RegisterGameObject(obj);
		obj->LoadFromJson(objectData);
		if (!parent.expired())
		{
			obj->SetParent(parent);
		}

		if (objectData.contains("children"))
		{
			CreateObjectsFromJsonRecursively(objectData["children"], obj);
		}
	}
}

void SceneManager::SaveSceneToFile(const std::string &filePath)
{
	nlohmann::json data;

	int iterator = 0;
	for (size_t i = 0; i < this->mainScene->gameObjects.size(); i++)
	{
		if (this->mainScene->gameObjects[i]->GetParent().expired())
		{
			this->mainScene->gameObjects[i]->SaveToJson(data["gameObjects"][iterator++]);
		}
	}

	std::ofstream outFile(filePath);
	outFile << data;
	outFile.close();
	this->currentScenePath = filePath;
}

void SceneManager::SetMainCameraInScene(std::shared_ptr<Scene>& scene)
{
	for (auto gameObject : scene->gameObjects) {
		if (CameraObject* newMainCamera = dynamic_cast<CameraObject*>(gameObject.get())) {
			if (newMainCamera != nullptr) {
				newMainCamera->SetMainCamera();
			}
		}
	}
}

void SceneManager::SaveSceneToCurrentFile()
{
	if (this->currentScenePath.empty())
	{
		Logger::Log("No current scene file path set. Use Save As to choose a file.");
		return;
	}
	SaveSceneToFile(this->currentScenePath);
}

void SceneManager::InitializeSoundBank(std::string pathToSoundFolder)
{
	AssetManager::GetInstance().InitializeSoundBank(pathToSoundFolder);
}

void SceneManager::AddSoundClipStandardFolder(std::string filename, std::string id)
{
	AssetManager::GetInstance().AddSoundClipStandardFolder(filename, id);
}

void SceneManager::AddSoundClip(std::string path, std::string id)
{
	AssetManager::GetInstance().AddSoundClip(path, id);
}

std::string SceneManager::GetPathToSoundFolder()
{
	return AssetManager::GetInstance().GetPathToSoundFolder();
}

SoundClip *SceneManager::GetSoundClip(std::string id)
{
	return AssetManager::GetInstance().GetSoundClip(id);
}
