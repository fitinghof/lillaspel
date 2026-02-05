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
	MeshObjData data = AssetManager::GetInstance().GetMeshObjData("TexBox/TextureCube.glb");

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

	////////////////

	DirectX::XMVECTOR pos1;
	pos1.m128_f32[0] = -2;
	pos1.m128_f32[1] = 0;
	pos1.m128_f32[2] = 0;

	DirectX::XMVECTOR pos2;
	pos2.m128_f32[0] = 2;
	pos2.m128_f32[1] = 0;
	pos2.m128_f32[2] = 0;

	auto rb1 = this->mainScene->CreateGameObjectOfType<RigidBody>();
	auto coll1 = this->mainScene->CreateGameObjectOfType<BoxCollider>();
	coll1.lock()->SetExtents(DirectX::XMFLOAT3(1, 1, 1));
	coll1.lock()->SetParent(rb1);
	rb1.lock()->transform.SetPosition(pos1);

	auto rb2 = this->mainScene->CreateGameObjectOfType<RigidBody>();
	auto coll2 = this->mainScene->CreateGameObjectOfType<BoxCollider>();
	coll2.lock()->SetExtents(DirectX::XMFLOAT3(1, 1, 1));
	coll2.lock()->SetParent(rb2);
	rb2.lock()->transform.SetPosition(pos2);

	rb1.lock()->Collision(rb2);
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
