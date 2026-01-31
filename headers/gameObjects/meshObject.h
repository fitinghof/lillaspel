#pragma once

#include "gameObjects/gameObject3D.h"
#include "gameObjects/mesh.h"
#include "rendering/renderQueue.h"

class MeshObject : public GameObject3D {
public:
	MeshObject();
	virtual ~MeshObject() = default;

	void SetMesh(Mesh* newMesh);
	Mesh* GetMesh();
private:
	Mesh* mesh; // Temp, it shouldn't actually own the mesh
};