#include "gameObjects/meshObject.h"

MeshObject::MeshObject() : mesh(nullptr)
{
	Logger::Log("Created a MeshObject.");
}

void MeshObject::SetMesh(Mesh* newMesh)
{
	this->mesh = newMesh;

	// Should do a check to make sure it isn't already in render queue

	RenderQueue::AddMeshObject(this);
}

Mesh* MeshObject::GetMesh()
{
	return this->mesh;
}

void MeshObject::Tick()
{
	static float rot = 0;
	this->transform.SetRotationRPY(0,0,rot += 0.0005f);
}
