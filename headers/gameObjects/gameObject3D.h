#pragma once

#include "gameObjects/gameObject.h"
#include "gameObjects/transform.h"

class GameObject3D : public GameObject {
public:
	GameObject3D();
	~GameObject3D() = default;

	Transform transform;
};