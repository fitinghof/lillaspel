#include "rendering/renderQueue.h"

RenderQueue* RenderQueue::instance = nullptr;

RenderQueue::RenderQueue(std::shared_ptr<std::vector<int>>& meshRenderQueue) : meshRenderQueue(meshRenderQueue)
{
	Logger::Log("Initializing RenderQueue.");

	if (instance) {
		throw std::runtime_error("Tried to create another RenderQueue, there can only be one.");
	}
	instance = this;
}

void RenderQueue::AddMeshObject()
{
	if (!instance) {
		throw std::runtime_error("Tried to add object to queue, but RenderQueue is not initialized.");
	}

	instance->meshRenderQueue->push_back(1);
}

void RenderQueue::RemoveMeshObject()
{
	if (!instance) {
		throw std::runtime_error("Tried to remove object to queue, but RenderQueue is not initialized.");
	}
}
