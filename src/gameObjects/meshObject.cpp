#include "gameObjects/meshObject.h"

MeshObject::MeshObject() : mesh()
{
	Logger::Log("Created a MeshObject.");
}

void MeshObject::SetMesh(MeshObjData newMesh)
{
	this->mesh = newMesh;

	// Should do a check to make sure it isn't already in render queue

	RenderQueue::AddMeshObject(this);
}

MeshObjData MeshObject::GetMesh()
{
	return this->mesh;
}

void MeshObject::Tick()
{
	GameObject3D::Tick();

	static float rot = 0;
	this->transform.SetRotationRPY(0,0,rot += 0.0005f);
}

void MeshObject::LoadFromJson(const nlohmann::json& data)
{
	this->GameObject3D::LoadFromJson(data);
}

void MeshObject::SaveToJson(nlohmann::json& data)
{
	this->GameObject3D::SaveToJson(data);

	data["type"] = "MeshObject";
}
