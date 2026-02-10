#include "gameObjects/room.h"
#include "core/assetManager.h"

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
		std::array<std::array<float, 2>, 4> wallpositions = {
			std::array<float, 2>({1, 0}),
			std::array<float, 2>({-1, 0}),
			std::array<float, 2>({0, 1}),
			std::array<float, 2>({0, -1})
		};
		std::array<float, 2> wallpos = wallpositions[i];

		wallpos[0] *= this->size / 2;
		wallpos[1] *= this->size / 2;

		Logger::Log("Wallpos z ", wallpos[1]);
		Logger::Log("Wallpos x ", wallpos[0]);

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
