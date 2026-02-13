#pragma once
#include "core/physics/collision.h"
#include "gameObjects/meshObject.h"
#include "core/physics/physicsQueue.h"

class Player : public RigidBody
{
public:
	Player() = default;
	~Player() = default;

	void Tick() override;
	void Start() override;

private:

};