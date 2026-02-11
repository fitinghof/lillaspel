#include "gameObjects/room.h"
#include "core/assetManager.h"

static const std::array<std::array<int, 2>, 4> wallpositions = {
	std::array<int, 2>({1, 0}),
	std::array<int, 2>({0, 1}),
	std::array<int, 2>({-1, 0}),
	std::array<int, 2>({0, -1}),
};

std::array<int, 2> Room::GetNeighborOffset(Room::WallIndex wallIndex) { return wallpositions[wallIndex]; }

void Room::Start() {
	Logger::Warn("room size ", this->size);
	{
		auto meshobjWeak = this->factory->CreateGameObjectOfType<MeshObject>();

		auto meshobj = meshobjWeak.lock();
		meshobj->SetParent(this->GetPtr());

		MeshObjData meshdata = AssetManager::GetInstance().GetMeshObjData("TexBox/TextureCube.glb:Mesh_0");
		meshobj->SetMesh(meshdata);

		meshobj->transform.SetPosition(DirectX::XMVectorSet(0, 5, 0, 0));
		meshobj->transform.SetScale(DirectX::XMVectorSet(this->size, 1, this->size, 1));

		this->roof = meshobj;
	}
	{
		auto meshobjWeak = this->factory->CreateGameObjectOfType<MeshObject>();

		auto meshobj = meshobjWeak.lock();
		meshobj->SetParent(this->GetPtr());

		MeshObjData meshdata = AssetManager::GetInstance().GetMeshObjData("TexBox/TextureCube.glb:Mesh_0");
		meshobj->SetMesh(meshdata);

		meshobj->transform.SetPosition(DirectX::XMVectorSet(0, -5, 0, 0));
		meshobj->transform.SetScale(DirectX::XMVectorSet(this->size, 1, this->size, 1));

		this->floor = meshobj;
	}
	for (size_t i = 0; i < 4; i++) {

		std::array<float, 2> wallpos = {wallpositions[i][0], wallpositions[i][1]};

		wallpos[0] *= this->size / 2;
		wallpos[1] *= this->size / 2;

		auto meshobjWeak = this->factory->CreateGameObjectOfType<MeshObject>();

		auto meshobj = meshobjWeak.lock();
		meshobj->SetParent(this->GetPtr());

		MeshObjData meshdata = AssetManager::GetInstance().GetMeshObjData("TexBox/TextureCube.glb:Mesh_0");
		meshobj->SetMesh(meshdata);

		meshobj->transform.SetPosition(DirectX::XMVectorSet(wallpos[0], 0, wallpos[1], 0));

		if (wallpos[0] == 0) {
			meshobj->transform.SetScale(DirectX::XMVectorSet(this->size, 5, 1, 1));
		} else {
			meshobj->transform.SetScale(DirectX::XMVectorSet(1, 5, this->size, 1));
		}

		this->walls[i] = meshobj;
	}
}

void Room::SetSize(float size) { Room::size = size; }

void Room::SetWallState(Room::WallIndex wallindex, bool active) {

	auto& wall = this->walls[wallindex];

	if (active == false && !wall.expired()) {
		this->factory->QueueDeleteGameObject(wall);
	} else if (!active && wall.expired()) {
		std::array<float, 2> wallpos = {wallpositions[wallindex][0], wallpositions[wallindex][1]};

		wallpos[0] *= this->size / 2;
		wallpos[1] *= this->size / 2;

		auto meshobjWeak = this->factory->CreateGameObjectOfType<MeshObject>();

		auto meshobj = meshobjWeak.lock();
		meshobj->SetParent(this->GetPtr());

		MeshObjData meshdata = AssetManager::GetInstance().GetMeshObjData("TexBox/TextureCube.glb:Mesh_0");
		meshobj->SetMesh(meshdata);

		meshobj->transform.SetPosition(DirectX::XMVectorSet(wallpos[0], 0, wallpos[1], 0));

		if (wallpos[0] == 0) {
			meshobj->transform.SetScale(DirectX::XMVectorSet(this->size, 5, 1, 1));
		} else {
			meshobj->transform.SetScale(DirectX::XMVectorSet(1, 5, this->size, 1));
		}

		this->walls[wallindex] = meshobj;
	}
}
