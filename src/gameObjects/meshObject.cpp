#include "gameObjects/meshObject.h"

MeshObject::MeshObject() : mesh(nullptr)
{
	Logger::Log("Created a MeshObject");
}

void MeshObject::SetMesh(Mesh* newMesh)
{
	this->mesh = newMesh;

	RenderQueue::AddMeshObject(this);
}

Mesh* MeshObject::GetMesh()
{
	return this->mesh;
}
