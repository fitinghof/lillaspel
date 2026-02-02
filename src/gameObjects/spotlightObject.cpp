#include "gameObjects/spotlightObject.h"

SpotlightObject::SpotlightObject()
{
	this->data = {};
	DirectX::XMStoreFloat3(&this->data.position, DirectX::XMVectorSet(0, 0, 0, 0));
	DirectX::XMStoreFloat3(&this->data.direction, DirectX::XMVectorSet(0, 0, 0, 0));
	DirectX::XMStoreFloat4(&this->data.color, DirectX::XMVectorSet(1, 1, 1, 1));
	this->data.intensity = 1;
	this->data.spotAngleRadians = 2 * DirectX::XMConvertToRadians(360);

	RenderQueue::AddLightObject(this);

	Logger::Log("Created a spotlight.");
}

void SpotlightObject::Tick()
{
	DirectX::XMStoreFloat3(&this->data.position, this->transform.GetPosition());
	DirectX::XMStoreFloat3(&this->data.direction, this->transform.GetDirectionVector());
}
