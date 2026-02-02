#pragma once

#include <vector>
#include <iostream>
#include "utilities/logger.h"

class MeshObject;

class RenderQueue {
public:
	RenderQueue(std::shared_ptr<std::vector<MeshObject*>> meshRenderQueue);
	~RenderQueue() = default;

	static void AddMeshObject(MeshObject* newMeshObject);
	static void RemoveMeshObject();
private:
	static RenderQueue* instance;

	std::shared_ptr<std::vector<MeshObject*>> meshRenderQueue;
};