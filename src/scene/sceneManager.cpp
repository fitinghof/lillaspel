#include "scene/sceneManager.h"

// Very good macro, please don't remove
#define NAMEOF(x) #x

SceneManager::SceneManager(Renderer* rend) : mainScene(nullptr), renderer(rend), objectFromString()
{
	this->objectFromString.RegisterType<GameObject>(NAMEOF(GameObject));
	this->objectFromString.RegisterType<GameObject3D>(NAMEOF(GameObject3D));
	this->objectFromString.RegisterType<MeshObject>(NAMEOF(MeshObject));
	this->objectFromString.RegisterType<SpotlightObject>(NAMEOF(SpotlightObject));
	this->objectFromString.RegisterType<CameraObject>(NAMEOF(CameraObject));
}

void SceneManager::SceneTick()
{
	if (!this->mainScene) return;

	this->mainScene->SceneTick();
}

void SceneManager::LoadScene()
{
	if (this->mainScene)
	{
		// TO DO: Delete old scene

		throw std::runtime_error("Tried to load another scene, unsupported.");
	}

	this->mainScene = std::make_unique<Scene>();

	LoadSceneFromFile("../../assets/scenes/test.json");
	SaveSceneToFile("../../assets/scenes/testresult.json");
}

void SceneManager::LoadSceneFromFile(const std::string& filePath)
{
	std::ifstream file(filePath);
	nlohmann::json data = nlohmann::json::parse(file);
	file.close();

	// Actual loading
	CreateObjectsFromJsonRecursively(data["gameObjects"], std::shared_ptr<GameObject>(nullptr));
}

void SceneManager::CreateObjectsFromJsonRecursively(const nlohmann::json& data, std::weak_ptr<GameObject> parent)
{
	for (const nlohmann::json& objectData : data) {
		//Logger::Log(objectData.dump());

		if (!objectData.contains("type")) {
			throw std::runtime_error("Failed to load scene: GameObject doesn't have a type.");
		}	

		GameObject* gameObjectPointer = static_cast<GameObject*>(objectFromString.Construct(objectData.at("type")));
		auto obj = std::shared_ptr<GameObject>(gameObjectPointer);
		this->mainScene->RegisterGameObject(obj);
		obj->LoadFromJson(objectData);
		if (!parent.expired()) {
			obj->SetParent(parent);
		}

		// temp
		if (auto p = dynamic_cast<MeshObject*>(gameObjectPointer)) {
			MeshObjData data = AssetManager::GetInstance().GetMeshObjData("TexBox/TextureCube.glb");
			p->SetMesh(data);
		}

		if (objectData.contains("children")) {
			CreateObjectsFromJsonRecursively(objectData["children"], obj);
		}
	}
}

void SceneManager::SaveSceneToFile(const std::string& filePath)
{
	nlohmann::json data;

	int iterator = 0;
	for (size_t i = 0; i < this->mainScene->gameObjects.size(); i++)
	{
		if (this->mainScene->gameObjects[i]->GetParent().expired()) {
			this->mainScene->gameObjects[i]->SaveToJson(data["gameObjects"][iterator++]);
		}
	}

	std::ofstream outFile(filePath);
	outFile << data;
	outFile.close();
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

SoundClip* SceneManager::GetSoundClip(std::string id)
{
	return AssetManager::GetInstance().GetSoundClip(id);
}
