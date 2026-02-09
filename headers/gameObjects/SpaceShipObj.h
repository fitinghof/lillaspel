#pragma once

#include "gameObjects/gameObject3D.h"

class SpaceShip : public GameObject3D {
public:
	SpaceShip();
	void CreateRoom(size_t x, size_t y);
	void Tick() override;

private:
	static const size_t SHIP_MAX_SIZE_X = 63;
	static const size_t SHIP_MAX_SIZE_Y = 63;
	std::array<std::array<std::weak_ptr<GameObject>, SHIP_MAX_SIZE_Y>, SHIP_MAX_SIZE_X> rooms{};
};