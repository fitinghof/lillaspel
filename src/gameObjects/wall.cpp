#include "gameObjects/wall.h"
#include "utilities/logger.h"
#include "gameObjects/room.h"
void Wall::OnObserve() { }

void Wall::OnInteract() { 
	
	Logger::Log("stop interacting with me!"); 

	auto parentWeak = this->GetParent();
	if (parentWeak.expired()) {
		Logger::Error("wall parent is dead, how in the funk?");
		return;
	}

	auto parent = std::static_pointer_cast<Room>(parentWeak.lock());

	
	parent->CreateRoom(static_cast<Room::WallIndex>(this->wallIndex));
}

void Wall::Start(){ 
	
	GameObject::Start();


}

void Wall::Tick()
{
		//this->transform.Rotate(0, Time::GetInstance().GetDeltaTime(), 0);

	if(GetAsyncKeyState('I'))
	{

		Logger::Log("NR OF COLLDEIRS: ", this->wallColliders.size());
		DirectX::XMVECTOR rotation = this->wallColliders[0].lock()->GetGlobalRotation();
		DirectX::XMVECTOR rotationQuat = this->wallColliders[0].lock()->transform.GetRotationQuaternion();

		std::shared_ptr<MeshObject> meshObj = this->wallColliders[0].lock()->GetMeshObjectChild().lock();
		DirectX::XMVECTOR meshObjRotation = meshObj->GetGlobalRotation();
		DirectX::XMVECTOR meshObjRotationQuat = meshObj->transform.GetRotationQuaternion();

		std::shared_ptr<BoxCollider> box = std::dynamic_pointer_cast<BoxCollider>(this->wallColliders[0].lock());
		if(box != nullptr)
		{
			Logger::Log("****SAT-DATA*****");
			SATData satData = box->GetSatData();

			for (int i = 0; i < 8; i++)
			{
				Logger::Log("pos: ", satData.positionData[i].x, ", ", satData.positionData[i].y, ", ", satData.positionData[i].z);
			}
			Logger::Log("*********");
		}

		Logger::Log("MESHOBJECT GLOBAL ROTATION: ", meshObjRotation.m128_f32[0], ", ", meshObjRotation.m128_f32[1], ", ", meshObjRotation.m128_f32[2]);
		Logger::Log("MESHOBJECT ROTATION QUAT: ", meshObjRotationQuat.m128_f32[0], ", ", meshObjRotationQuat.m128_f32[1], ", ", meshObjRotationQuat.m128_f32[2]);
		Logger::Log("COLLIDER 2 GLOBAL ROTATION: ", rotation.m128_f32[0], ", ", rotation.m128_f32[1], ", ", rotation.m128_f32[2]);
		Logger::Log("COLLIDER 2 ROTATION QUAT: ", rotationQuat.m128_f32[0], ", ", rotationQuat.m128_f32[1], ", ", rotationQuat.m128_f32[2]);
		Logger::Log("-------------------------------------------------------");
	}
}

void Wall::SetWAllIndex(int wallIndex) { this->wallIndex = wallIndex; }


void Wall::SpawnInteractables() {
	auto colliderobjWeak = this->factory->CreateGameObjectOfType<BoxCollider>();

	auto colliderobj = colliderobjWeak.lock();
	DirectX::XMFLOAT3 pos(0.0f, 3.0f, 4.5f);
	colliderobj->transform.SetPosition(DirectX::XMLoadFloat3(&pos));
	DirectX::XMFLOAT3 scale(0.750f, 0.750f, 0.250f);
	colliderobj->transform.SetScale(DirectX::XMLoadFloat3(&scale));
	colliderobj->SetParent(this->GetPtr());

	this->interactable = colliderobj;
}

void Wall::SpawnWallColliders(int wallStateIndex) {
	for (auto colliders: this->wallColliders) {
		this->factory->QueueDeleteGameObject(colliders);
	}
	this->wallColliders.clear();
	Room::WallState wallState = static_cast<Room::WallState>(wallStateIndex);
	switch (wallState) {
	case (Room::WallState::window || Room::WallState::solid):
		{
		auto colliderobjWeak = this->factory->CreateGameObjectOfType<BoxCollider>();
		auto colliderobj = colliderobjWeak.lock();
		DirectX::XMFLOAT3 pos(0.0f, 3.0f, 4.75f);
		colliderobj->transform.SetPosition(DirectX::XMLoadFloat3(&pos));
		DirectX::XMFLOAT3 scale(5.0f, 2.5f, 0.250f);
		colliderobj->transform.SetScale(DirectX::XMLoadFloat3(&scale));
		colliderobj->SetParent(this->GetPtr());

		this->wallColliders.push_back(colliderobj);
	}
		break;

	case (Room::WallState::door): {
		auto colliderobjWeak = this->factory->CreateGameObjectOfType<BoxCollider>();
		auto colliderobj = colliderobjWeak.lock();
		DirectX::XMFLOAT3 pos(3.25f, 3.0f, 4.75f);
		colliderobj->transform.SetPosition(DirectX::XMLoadFloat3(&pos));
		DirectX::XMFLOAT3 scale(1.75f, 2.5f, 0.250f);
		colliderobj->transform.SetScale(DirectX::XMLoadFloat3(&scale));
		colliderobj->SetParent(this->GetPtr());

		this->wallColliders.push_back(colliderobj);
	}
	{
		auto colliderobjWeak = this->factory->CreateGameObjectOfType<BoxCollider>();
		auto colliderobj = colliderobjWeak.lock();
		DirectX::XMFLOAT3 pos(-3.25f, 3.0f, 4.75f);
		colliderobj->transform.SetPosition(DirectX::XMLoadFloat3(&pos));
		DirectX::XMFLOAT3 scale(1.75f, 2.5f, 0.250f);
		colliderobj->transform.SetScale(DirectX::XMLoadFloat3(&scale));
		colliderobj->SetParent(this->GetPtr());

		this->wallColliders.push_back(colliderobj);
	}
		break;

	default:
		break;
	}
}

void Wall::SetWallState(int wallState) {
	
	MeshObjData meshdata = AssetManager::GetInstance().GetMeshObjData(
	this->wallMeshIdentifiers[wallState]);
	this->SetMesh(meshdata);
	this->SpawnWallColliders(wallState);
	Room::WallState wallStateEnum = static_cast<Room::WallState>(wallState);
	switch (wallStateEnum) {
	case (Room::WallState::window):
		//this->SpawnInteractables();
		break;

	case (Room::WallState::solid):
		//this->SpawnInteractables();
		break;

	case (Room::WallState::door):
		this->RemoveInteractables();
		break;

	default:
		break;
	}
}

void Wall::RemoveInteractables() {

	this->factory->QueueDeleteGameObject(this->interactable);
}