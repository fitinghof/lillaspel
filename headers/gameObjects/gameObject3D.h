#pragma once

#include "gameObjects/gameObject.h"
#include "gameObjects/transform.h"

class GameObject3D : public GameObject {
public:
	GameObject3D();
	virtual ~GameObject3D() = default;

	virtual void Tick() override;

	Transform transform;
};