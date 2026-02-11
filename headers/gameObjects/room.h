#pragma once
#include "gameObjects/gameObject3D.h"
#include "gameObjects/meshObject.h"

class WallIndex {

};

class Room : public GameObject3D {
public:
	enum WallIndex {
		North,
		East,
		South, 
		West,
	};

	static std::array<int, 2> GetNeighborOffset(WallIndex wallIndex);
	void Start() override;
	static void SetSize(float size);
	void SetWallState(Room::WallIndex wall, bool active);

private:

	inline static float size;
	std::weak_ptr<MeshObject> roof;
	std::weak_ptr<MeshObject> floor;
	std::array<std::weak_ptr<MeshObject>, 4> walls;
};