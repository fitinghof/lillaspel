#include "scene/sceneManager.h"
#include "gameObjects/objectLoader.h"

SceneManager::SceneManager(Renderer* rend) : mainScene(nullptr), renderer(rend), assetManager(rend->GetDevice())
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
	MeshObjData data = this->renderer->assetManager.GetMeshObjData("Box/cube.glb");

	auto firstMesh = this->mainScene->CreateGameObjectOfType<MeshObject>();
	firstMesh.lock()->transform.SetPosition(DirectX::XMVectorSet(5, 0, 10, 1));
	firstMesh.lock()->SetMesh(data);

	auto secondMesh = this->mainScene->CreateGameObjectOfType<MeshObject>();
	secondMesh.lock()->transform.SetPosition(DirectX::XMVectorSet(0, 0, 10, 1));	
	secondMesh.lock()->SetMesh(data);
	


	//auto thirdMesh = this->mainScene->CreateGameObjectOfType<MeshObject>();
	//thirdMesh.lock()->transform.SetPosition(DirectX::XMVectorSet(-5, 0, 10, 1));
	//thirdMesh.lock()->SetMesh(this->tempMeshes[2].get());

	auto light = this->mainScene->CreateGameObjectOfType<SpotlightObject>();

	auto light2 = this->mainScene->CreateGameObjectOfType<SpotlightObject>();
	light2.lock()->transform.SetPosition(DirectX::XMVectorSet(0, 0, 10, 1));
	DirectX::XMStoreFloat4(&light2.lock()->data.color, DirectX::XMVectorSet(0, 0, 1, 1));
	Logger::Log("Loaded scene");
}
