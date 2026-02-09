#pragma once
#include "core/physics/collision.h"
#include "gameObjects/meshObject.h"

class TestPlayer : public BoxCollider
{
public:
	TestPlayer();
	~TestPlayer();

	void Tick() override;
	void Start() override;

private:

};