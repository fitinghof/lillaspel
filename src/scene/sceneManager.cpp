#include "scene/sceneManager.h"

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
	if (this->mainScene)
	{
		// TO DO: Delete old scene

		throw std::runtime_error("Tried to load another scene, unsupported.");
	}

	this->mainScene = std::unique_ptr<Scene>(new Scene());

	std::weak_ptr<CameraObject> camera = this->mainScene->CreateGameObjectOfType<CameraObject>();


	// Temporary meshes

	Vertex vertexData[] = {
	{-1, -1, 0,		0.0f, 0.0f, -1.0f,	1.0f, 1.0f},
	{-1,  1, 0,		0.0f, 0.0f, -1.0f,	1.0f, 1.0f},
	{ 1, -1, 0,		0.0f, 0.0f, -1.0f,	1.0f, 1.0f},
	{ 1,  1, 0,		0.0f, 0.0f, -1.0f,	1.0f, 1.0f}
	};

	VertexBuffer tempVBuffer;
	tempVBuffer.Init(renderer->GetDevice(), sizeof(Vertex), 4, vertexData);

	uint32_t indices[] = {
		0,1,2,1,3,2
	};

	IndexBuffer tempIBuffer;
	tempIBuffer.Init(renderer->GetDevice(), 6, indices);

	this->tempMesh = std::unique_ptr<Mesh>(new Mesh(std::move(tempVBuffer), std::move(tempIBuffer), std::vector<SubMesh>()));


	// Create temporary meshObjects

	auto firstMesh = this->mainScene->CreateGameObjectOfType<MeshObject>();
	firstMesh.lock()->transform.SetPosition(DirectX::XMVectorSet(5, 0, 10, 1));
	firstMesh.lock()->SetMesh(this->tempMesh.get());
	
	auto secondMesh = this->mainScene->CreateGameObjectOfType<MeshObject>();
	secondMesh.lock()->transform.SetPosition(DirectX::XMVectorSet(0, 0, 10, 1));
	secondMesh.lock()->SetMesh(this->tempMesh.get());

	secondMesh.lock()->GetGlobalPosition();
}
