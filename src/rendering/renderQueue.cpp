#include "rendering/renderQueue.h"
#include "gameObjects/meshObject.h"

RenderQueue* RenderQueue::instance = nullptr;

RenderQueue::RenderQueue(std::shared_ptr<std::vector<MeshObject*>> meshRenderQueue, std::shared_ptr<std::vector<SpotlightObject*>> lightRenderQueue)
	: meshRenderQueue(meshRenderQueue), lightRenderQueue(lightRenderQueue)
{
	Logger::Log("Initializing RenderQueue.");

	if (instance) {
		throw std::runtime_error("Tried to create another RenderQueue, there can only be one.");
	}

	instance = this;
}

void RenderQueue::AddMeshObject(MeshObject* newMeshObject)
{
	if (!instance) {
		throw std::runtime_error("Tried to add object to queue, but RenderQueue is not initialized.");
	}

	if (!instance->meshRenderQueue)
	{
		throw std::runtime_error("meshRenderQueue is null");
	}

	instance->meshRenderQueue->push_back(newMeshObject);
}

void RenderQueue::RemoveMeshObject()
{
	if (!instance) {
		throw std::runtime_error("Tried to remove object to queue, but RenderQueue is not initialized.");
	}
}

void RenderQueue::AddLightObject(SpotlightObject* newSpotlightObject)
{
	if (!instance) {
		throw std::runtime_error("Tried to add light to queue, but RenderQueue is not initialized.");
	}

	if (!instance->lightRenderQueue)
	{
		throw std::runtime_error("lightRenderQueue is null");
	}

	instance->lightRenderQueue->push_back(newSpotlightObject);
}
