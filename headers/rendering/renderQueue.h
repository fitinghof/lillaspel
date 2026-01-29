#pragma once

#include <vector>
#include <iostream>
#include "utilities/logger.h"

class RenderQueue {
public:
	RenderQueue(std::shared_ptr<std::vector<int>>& meshRenderQueue);
	~RenderQueue() = default;

	static void AddMeshObject();
	static void RemoveMeshObject();
private:
	static RenderQueue* instance;

	std::shared_ptr<std::vector<int>> meshRenderQueue;
};