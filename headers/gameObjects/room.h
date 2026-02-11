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
	static void SetSize(float size);
	void Start() override;
	void Tick() override;

	void CreateRoom(WallIndex wallIndex);
	void SetPosition(size_t x, size_t y);
	void SetWallState(Room::WallIndex wall, bool active);
	void SetParent(std::weak_ptr<GameObject> parent) override;

private:


	std::array<size_t, 2> pos;
	inline static float size;
	std::weak_ptr<MeshObject> roof;
	std::weak_ptr<MeshObject> floor;
	std::array<std::weak_ptr<MeshObject>, 4> walls;
};