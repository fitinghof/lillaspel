#pragma once
#include "gameObjects/gameObject3D.h"
#include "gameObjects/meshObject.h"

class Room : public GameObject3D {
public:
	void Start() override;
	static void SetSize(float size);

private:

	inline static float size;
	std::weak_ptr<MeshObject> roof;
	std::weak_ptr<MeshObject> floor;
	std::array<std::weak_ptr<MeshObject>, 4> walls;
};