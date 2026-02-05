#include "scene/sceneManager.h"
#include "gameObjects/objectLoader.h"

SceneManager::SceneManager(Renderer* rend) : mainScene(nullptr), renderer(rend)
{
}

void SceneManager::SceneTick()
{
	if (!this->mainScene) return;

	this->mainScene->SceneTick();
}

void SceneManager::LoadScene()
{
	this->mainScene = std::make_unique<Scene>();

	this->mainScene->CreateGameObjectOfType<CameraObject>();
	// So basically right now this is the temporary place to create scenes, before we can load them from file
	MeshObjData data = AssetManager::GetInstance().GetMeshObjData("CS/sas__cs2_agent_model_blue.glb:Mesh_2");

	auto firstMesh = this->mainScene->CreateGameObjectOfType<MeshObject>();
	firstMesh.lock()->transform.SetPosition(DirectX::XMVectorSet(5, 0, 10, 1));
	firstMesh.lock()->SetMesh(data);

	auto secondMesh = this->mainScene->CreateGameObjectOfType<MeshObject>();
	secondMesh.lock()->transform.SetPosition(DirectX::XMVectorSet(0, 0, 10, 1));	
	secondMesh.lock()->SetMesh(data);
	

	auto thirdMesh = this->mainScene->CreateGameObjectOfType<MeshObject>();
	thirdMesh.lock()->transform.SetPosition(DirectX::XMVectorSet(-5, 0, 10, 1));
	thirdMesh.lock()->SetMesh(data);

	auto emptyObj = this->mainScene->CreateGameObjectOfType<GameObject>();

	thirdMesh.lock()->SetParent(secondMesh);
	emptyObj.lock()->SetParent(thirdMesh);
	firstMesh.lock()->SetParent(emptyObj);
	thirdMesh.lock()->transform.SetPosition(DirectX::XMVectorSet(0, 0, 5, 1));
	firstMesh.lock()->transform.SetPosition(DirectX::XMVectorSet(0, 0, 5, 1));
	secondMesh.lock()->transform.SetPosition(DirectX::XMVectorSet(0, 0, 15, 1));

	auto light = this->mainScene->CreateGameObjectOfType<SpotlightObject>();

	auto light2 = this->mainScene->CreateGameObjectOfType<SpotlightObject>();
	light2.lock()->transform.SetPosition(DirectX::XMVectorSet(0, 0, 10, 1));
	DirectX::XMStoreFloat4(&light2.lock()->data.color, DirectX::XMVectorSet(0, 0, 1, 1));

	Logger::Log(this->mainScene->GetNumberOfGameObjects());
	this->mainScene->QueueDeleteGameObject(light2);
	Logger::Log("Loaded scene");
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
