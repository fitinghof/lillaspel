#include "scene/sceneManager.h"
#include "gameObjects/objectLoader.h"

SceneManager::SceneManager(Renderer* rend) : mainScene(nullptr), renderer(rend), objectFromString()
{
}

void SceneManager::SceneTick()
{
	if (!this->mainScene) return;

	this->mainScene->SceneTick();
}

void SceneManager::LoadScene()
{
	// So basically right now this is the temporary place to create scenes, before we can load them from file

	if (this->mainScene)
	{
		// TO DO: Delete old scene

		throw std::runtime_error("Tried to load another scene, unsupported.");
	}

	this->mainScene = std::unique_ptr<Scene>(new Scene());

	LoadSceneFromFile("../../assets/scenes/test.json");

	std::weak_ptr<CameraObject> camera = this->mainScene->CreateGameObjectOfType<CameraObject>();


	//// Temporary meshes

	//std::unique_ptr<Mesh> glbMesh = std::unique_ptr<Mesh>(new Mesh());
	//ObjectLoader loader;
	//loader.LoadGltf(*glbMesh.get(), "../../assets/Box/cube.glb", this->renderer->GetDevice());

	//this->tempMeshes.push_back(std::move(glbMesh));

	//Vertex vertexData[] = {
	//{-1, -1, 0,		0.0f, 0.0f, -1.0f,		0.0f, 1.0f},
	//{-1,  1, 0,		0.0f, 0.0f, -1.0f,		0.0f, 0.0f},
	//{ 1, -1, 0,		0.0f, 0.0f, -1.0f,		1.0f, 1.0f},
	//{ 1,  1, 0,		0.0f, 0.0f, -1.0f,		1.0f, 0.0f}
	//};

	//VertexBuffer tempVBuffer;
	//tempVBuffer.Init(renderer->GetDevice(), sizeof(Vertex), 4, vertexData);

	//uint32_t indices[] = {
	//	0,1,2,1,3,2
	//};

	//IndexBuffer tempIBuffer;
	//tempIBuffer.Init(renderer->GetDevice(), 6, indices);

	//std::vector<SubMesh> quadSubMeshes;
	//quadSubMeshes.push_back(SubMesh(0, 6));
	//this->tempMeshes.push_back(std::unique_ptr<Mesh>(new Mesh(std::move(tempVBuffer), std::move(tempIBuffer), std::move(quadSubMeshes))));




	//Vertex cubeVertexData[] = {
	//// Front
	//{ -1, -1,  1,   0,  0,  1,   0, 1 },
	//{ -1,  1,  1,   0,  0,  1,   0, 0 },
	//{  1, -1,  1,   0,  0,  1,   1, 1 },
	//{  1,  1,  1,   0,  0,  1,   1, 0 },
	//// Back
	//{  1, -1, -1,   0,  0, -1,   0, 1 },
	//{  1,  1, -1,   0,  0, -1,   0, 0 },
	//{ -1, -1, -1,   0,  0, -1,   1, 1 },
	//{ -1,  1, -1,   0,  0, -1,   1, 0 },
	//// Left
	//{ -1, -1, -1,  -1,  0,  0,   0, 1 },
	//{ -1,  1, -1,  -1,  0,  0,   0, 0 },
	//{ -1, -1,  1,  -1,  0,  0,   1, 1 },
	//{ -1,  1,  1,  -1,  0,  0,   1, 0 },
	//// Right
	//{  1, -1,  1,   1,  0,  0,   0, 1 },
	//{  1,  1,  1,   1,  0,  0,   0, 0 },
	//{  1, -1, -1,   1,  0,  0,   1, 1 },
	//{  1,  1, -1,   1,  0,  0,   1, 0 },
	////Top
	//{ -1,  1,  1,   0,  1,  0,   0, 1 },
	//{ -1,  1, -1,   0,  1,  0,   0, 0 },
	//{  1,  1,  1,   0,  1,  0,   1, 1 },
	//{  1,  1, -1,   0,  1,  0,   1, 0 },
	////Bottom
	//{ -1, -1, -1,   0, -1,  0,   0, 1 },
	//{ -1, -1,  1,   0, -1,  0,   0, 0 },
	//{  1, -1, -1,   0, -1,  0,   1, 1 },
	//{  1, -1,  1,   0, -1,  0,   1, 0 },
	//};

	//VertexBuffer cubeTempVBuffer;
	//cubeTempVBuffer.Init(renderer->GetDevice(), sizeof(Vertex), 24, cubeVertexData);

	//uint32_t cubeIndices[] = {
	//	// Front
	//	0,  1,  2,   2,  1,  3,
	//	// Back
	//	4,  5,  6,   6,  5,  7,
	//	// Left
	//	8,  9, 10,  10,  9, 11,
	//	// Right
	//	12, 13, 14,  14, 13, 15,
	//	// Top
	//	16, 17, 18,  18, 17, 19,
	//	// Bottom
	//	20, 21, 22,  22, 21, 23
	//};

	//IndexBuffer cubeTempIBuffer;
	//cubeTempIBuffer.Init(renderer->GetDevice(), 36, cubeIndices);

	//std::vector<SubMesh> cubeSubMeshes;
	//cubeSubMeshes.push_back(SubMesh(0, 36));

	//this->tempMeshes.push_back(std::unique_ptr<Mesh>(new Mesh(std::move(cubeTempVBuffer), std::move(cubeTempIBuffer), std::move(cubeSubMeshes))));


	//// Create temporary meshObjects

	//auto firstMesh = this->mainScene->CreateGameObjectOfType<MeshObject>();
	//firstMesh.lock()->SetMesh(this->tempMeshes[1].get());
	//
	//auto secondMesh = this->mainScene->CreateGameObjectOfType<MeshObject>();
	//secondMesh.lock()->SetMesh(this->tempMeshes[0].get());

	//auto thirdMesh = this->mainScene->CreateGameObjectOfType<MeshObject>();
	//thirdMesh.lock()->SetMesh(this->tempMeshes[2].get());

	//auto emptyObj = this->mainScene->CreateGameObjectOfType<GameObject>();

	//thirdMesh.lock()->SetParent(secondMesh);
	//emptyObj.lock()->SetParent(thirdMesh);
	//firstMesh.lock()->SetParent(emptyObj);
	//thirdMesh.lock()->transform.SetPosition(DirectX::XMVectorSet(0, 0, 5, 1));
	//firstMesh.lock()->transform.SetPosition(DirectX::XMVectorSet(0, 0, 5, 1));
	//secondMesh.lock()->transform.SetPosition(DirectX::XMVectorSet(0, 0, 15, 1));

	//auto light = this->mainScene->CreateGameObjectOfType<SpotlightObject>();

	//auto light2 = this->mainScene->CreateGameObjectOfType<SpotlightObject>();
	//light2.lock()->transform.SetPosition(DirectX::XMVectorSet(0, 0, 10, 1));
	//DirectX::XMStoreFloat4(&light2.lock()->data.color, DirectX::XMVectorSet(0, 0, 1, 1));

	//Logger::Log(this->mainScene->GetNumberOfGameObjects());
	//this->mainScene->QueueDeleteGameObject(light2);
}

void SceneManager::LoadSceneFromFile(const std::string& filePath)
{
	std::weak_ptr<CameraObject> camera = this->mainScene->CreateGameObjectOfType<CameraObject>();

	// Temp
	std::unique_ptr<Mesh> glbMesh = std::unique_ptr<Mesh>(new Mesh());
	ObjectLoader loader;
	loader.LoadGltf(*glbMesh.get(), "../../assets/Box/cube.glb", this->renderer->GetDevice());

	this->tempMeshes.push_back(std::move(glbMesh));
	// --

	std::ifstream file(filePath);
	nlohmann::json data = nlohmann::json::parse(file);
	file.close();

	//Logger::Log(data.dump(4));

	objectFromString.RegisterClassW<GameObject>("GameObject");
	objectFromString.RegisterClassW<GameObject3D>("GameObject3D");
	objectFromString.RegisterClassW<MeshObject>("MeshObject");

	for (const nlohmann::json& objectData : data["gameObjects"]) {

		Logger::Log(objectData.dump());

		GameObject* gameObjectPointer = static_cast<GameObject*>(objectFromString.Construct(objectData.at("type")));		
		auto obj = std::shared_ptr<GameObject>(gameObjectPointer);
		Logger::Log(gameObjectPointer);
		this->mainScene->RegisterGameObject(obj);
		obj->LoadFromJson(objectData);

		// temp
		if (auto p = dynamic_cast<MeshObject*>(gameObjectPointer)) {
			p->SetMesh(this->tempMeshes[0].get());
		}
	}
}

void SceneManager::InitializeSoundBank(std::string pathToSoundFolder)
{
	this->assetManager.InitializeSoundBank(pathToSoundFolder);
}

void SceneManager::AddSoundClipStandardFolder(std::string filename, std::string id)
{
	this->assetManager.AddSoundClipStandardFolder(filename, id);
}

void SceneManager::AddSoundClip(std::string path, std::string id)
{
	this->assetManager.AddSoundClip(path, id);
}

std::string SceneManager::GetPathToSoundFolder()
{
	return this->assetManager.GetPathToSoundFolder();
}

SoundClip* SceneManager::GetSoundClip(std::string id)
{
	return this->assetManager.GetSoundClip(id);
}
