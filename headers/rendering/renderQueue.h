#pragma once
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <vector>
#include <iostream>
#include "utilities/logger.h"

class MeshObject;
class SpotlightObject;

class RenderQueue {
public:
	RenderQueue(std::shared_ptr<std::vector<MeshObject*>> meshRenderQueue, std::shared_ptr<std::vector<SpotlightObject*>> lightRenderQueue);
	~RenderQueue() = default;

	static void AddMeshObject(MeshObject* newMeshObject);
	static void RemoveMeshObject();

	static void AddLightObject(SpotlightObject* newSpotlightObject);

	static void ClearAllQueues();
private:
	static RenderQueue* instance;

	std::shared_ptr<std::vector<MeshObject*>> meshRenderQueue;
	std::shared_ptr<std::vector<SpotlightObject*>> lightRenderQueue;
};