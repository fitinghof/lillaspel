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
	this->objectFromString.RegisterType<DebugCamera>(NAMEOF(DebugCamera));
}

void SceneManager::SceneTick()
{
	if (!this->mainScene) return;

	this->mainScene->SceneTick();
	this->AudioManagerTick();
}

void SceneManager::LoadScene()
{
	if (this->mainScene)
	{
		// TO DO: Delete old scene

		throw std::runtime_error("Tried to load another scene, unsupported.");
	}

	this->mainScene = std::make_unique<Scene>();

	LoadSceneFromFile("../../assets/scenes/testresult.json");
	//SaveSceneToFile("../../assets/scenes/testresult.json");
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

		//// temp
		//if (auto p = dynamic_cast<MeshObject*>(gameObjectPointer)) {
		//	MeshObjData data = AssetManager::GetInstance().GetMeshObjData("TexBox/TextureCube.glb");
		//	p->SetMesh(data);
		//}

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

void SceneManager::AudioManagerTick()
{
	this->audioManager.Tick();
}

void SceneManager::InitializeMusicTrackManager(std::string pathToMusicFolder)
{
	this->audioManager.InitializeMusicTrackManager(pathToMusicFolder);
}

void SceneManager::AddMusicTrackStandardFolder(std::string filename, std::string id)
{
	this->audioManager.AddMusicTrackStandardFolder(filename, id);
}

void SceneManager::AddMusicTrack(std::string path, std::string id)
{
	this->audioManager.AddMusicTrack(path, id);
}

void SceneManager::PlayMusicTrack(std::string id)
{
	this->audioManager.Play(id);
}

void SceneManager::StopMusicTrack(std::string id)
{
	this->audioManager.Stop(id);
}

void SceneManager::FadeInPlayMusicTrack(std::string id, float startGain, float seconds)
{
	this->audioManager.FadeInPlay(id, startGain, seconds);
}

void SceneManager::FadeOutStopMusicTrack(std::string id, float seconds)
{
	this->audioManager.FadeOutStop(id, seconds);
}

void SceneManager::GetMusicTrackSourceState(std::string id, ALint& sourceState)
{
	this->audioManager.GetMusicTrackSourceState(id, sourceState);
}

void SceneManager::SetMusicTrackGain(std::string id, float gain)
{
	this->audioManager.SetGain(id, gain);
}

void SceneManager::SetMasterMusicGain(float gain)
{
	this->audioManager.SetMusicGain(gain);
}

MusicTrack* SceneManager::GetMusicTrack(std::string id)
{
	return this->audioManager.GetMusicTrack(id);
}
