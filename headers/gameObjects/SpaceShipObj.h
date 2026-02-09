#pragma once
#include "gameObjects/gameObject3D.h"



class SpaceShip : public GameObject3D {
public:


private:
	static const size_t SHIP_MAX_SIZE_X = 63;
	static const size_t SHIP_MAX_SIZE_Y = 64;
	std::array<std::array<std::weak_ptr<GameObject>, SHIP_MAX_SIZE_X>, SHIP_MAX_SIZE_Y> rooms;
};