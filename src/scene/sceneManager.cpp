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
	this->objectFromString.RegisterType<SpaceShip>(NAMEOF(SpaceShip));
	this->objectFromString.RegisterType<BoxCollider>(NAMEOF(BoxCollider));
	this->objectFromString.RegisterType<SphereCollider>(NAMEOF(SphereCollider));
	this->objectFromString.RegisterType<RigidBody>(NAMEOF(RigidBody));
	this->objectFromString.RegisterType<SoundSourceObject>(NAMEOF(SoundSourceObject));

	this->objectFromString.RegisterType<TestPlayer>(NAMEOF(TestPlayer));

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
		LoadSceneFromFile("../../../../assets/scenes/testresult.json");
		break;
	default:
		break;
	}

	//Temp

	CreateNewScene(this->mainScene);

	auto cam = this->mainScene->CreateGameObjectOfType<CameraObject>();

	//////////////////////////////////////////////////

	// std::shared_ptr<MeshObject> mesh = this->mainScene->CreateGameObjectOfType<MeshObject>().lock();
	// std::shared_ptr<TestPlayer> player = this->mainScene->CreateGameObjectOfType<TestPlayer>().lock();
	// mesh->SetParent(player);

	// //////////////////////////////////////////////////

	// DirectX::XMVECTOR pos1;
	// pos1.m128_f32[0] = -3.0f;
	// pos1.m128_f32[1] = 0;
	// pos1.m128_f32[2] = 0;

	// DirectX::XMVECTOR pos2;
	// pos2.m128_f32[0] = 2;
	// pos2.m128_f32[1] = 0;
	// pos2.m128_f32[2] = 0;

	// auto coll1 = this->mainScene->CreateGameObjectOfType<BoxCollider>();
	// auto coll2 = this->mainScene->CreateGameObjectOfType<BoxCollider>();

	// std::shared_ptr<BoxCollider> Coll1 = coll1.lock();
	// Coll1->solid = true;
	// Coll1->dynamic = true;
	// Coll1->SetPosition(DirectX::XMFLOAT3(4, 0, 0));
	// Coll1->SetExtents(DirectX::XMFLOAT3(1, 1, 1));

	// std::shared_ptr<BoxCollider> Coll2 = coll2.lock();
	// Coll2->solid = true;
	// Coll2->dynamic = true;
	// Coll2->SetPosition(DirectX::XMFLOAT3(2.5f, 0, 0));
	// Coll2->SetExtents(DirectX::XMFLOAT3(1, 1, 1));

	///////////////////////////////////////////////////////////////////

	//auto coll1 = this->mainScene->CreateGameObjectOfType<SphereCollider>();
	//auto coll2 = this->mainScene->CreateGameObjectOfType<SphereCollider>();

	//std::shared_ptr<SphereCollider> Coll1 = coll1.lock();
	//Coll1->solid = true;
	//Coll1->dynamic = true;
	//Coll1->SetPosition(DirectX::XMFLOAT3(4, 0, 0));
	//Coll1->SetDiameter(1);

	//std::shared_ptr<SphereCollider> Coll2 = coll2.lock();
	//Coll2->solid = true;
	//Coll2->dynamic = true;
	//Coll2->SetPosition(DirectX::XMFLOAT3(3.4f, 0, 0));
	//Coll2->SetDiameter(1);

	//Logger::Log("sphere1 center: " + std::to_string(Coll1->GetGlobalPosition().m128_f32[0]) + ", " + std::to_string(Coll1->GetGlobalPosition().m128_f32[1]) + ", " + std::to_string(Coll1->GetGlobalPosition().m128_f32[2]));
	//Logger::Log("sphere2 center: " + std::to_string(Coll2->GetGlobalPosition().m128_f32[0]) + ", " + std::to_string(Coll2->GetGlobalPosition().m128_f32[1]) + ", " + std::to_string(Coll2->GetGlobalPosition().m128_f32[2]));

	// if (Coll1->Collision(Coll2.get())) Logger::Log("true");
	// else Logger::Log("false");

	// Coll1->Tick();
	// Coll2->Tick();

	// Logger::Log("*********** END ************");

	// if (Coll1->Collision(Coll2.get())) Logger::Log("true");
	// else Logger::Log("false");

	// Coll1->Tick();
	// Coll2->Tick();

	// Logger::Log("*********** END ************");

	//Logger::Log("sphere1 center: " + std::to_string(Coll1->GetGlobalPosition().m128_f32[0]) + ", " + std::to_string(Coll1->GetGlobalPosition().m128_f32[1]) + ", " + std::to_string(Coll1->GetGlobalPosition().m128_f32[2]));
	//Logger::Log("sphere2 center: " + std::to_string(Coll2->GetGlobalPosition().m128_f32[0]) + ", " + std::to_string(Coll2->GetGlobalPosition().m128_f32[1]) + ", " + std::to_string(Coll2->GetGlobalPosition().m128_f32[2]));

	////////////////////////////////////////////////////////

	//auto rb1 = this->mainScene->CreateGameObjectOfType<RigidBody>();
	//auto coll1 = this->mainScene->CreateGameObjectOfType<BoxCollider>();
	//coll1.lock()->SetExtents(DirectX::XMFLOAT3(1, 1, 1));
	//coll1.lock()->SetParent(rb1);
	//coll1.lock()->SetPosition(DirectX::XMFLOAT3(0, 0, 0)); //to make sure cornersArray is rebuilt
	//rb1.lock()->transform.SetPosition(pos1);

	//auto rb2 = this->mainScene->CreateGameObjectOfType<RigidBody>();
	//auto coll2 = this->mainScene->CreateGameObjectOfType<BoxCollider>();
	//coll2.lock()->SetExtents(DirectX::XMFLOAT3(1, 1, 1));
	//coll2.lock()->SetParent(rb2);
	//coll2.lock()->SetPosition(DirectX::XMFLOAT3(0, 0, 0));
	//rb2.lock()->transform.SetPosition(pos2);

	//if (rb1.lock()->Collision(rb2)) Logger::Log("true");
	//else Logger::Log("false");
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

	//Logger::Log(this->mainScene->GetNumberOfGameObjects());
	//this->mainScene->QueueDeleteGameObject(light2);
	//Logger::Log("Loaded scene");

	////////////////

	DirectX::XMVECTOR pos1;
	pos1.m128_f32[0] = -2.0f;
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

	if(rb1.lock()->Collision(rb2)) Logger::Log("true");
	else Logger::Log("false");
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

MusicTrack* SceneManager::GetMusicTrack(std::string id)
{
	return this->audioManager.GetMusicTrack(id);
}
