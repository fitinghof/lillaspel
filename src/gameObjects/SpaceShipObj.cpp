#include "gameObjects/SpaceShipObj.h"
#include "core/assetManager.h"
#include "gameObjects/meshObject.h"
#include "imgui.h"

SpaceShip::SpaceShip() : GameObject3D() { 
	Room::SetSize(this->ROOM_SIZE); 
}

void SpaceShip::CreateRoom(size_t x, size_t y) {
	if (x < SHIP_MAX_SIZE_X && y < SHIP_MAX_SIZE_Y && rooms[x][y].expired()) {

		std::weak_ptr<Room> room = this->factory->CreateGameObjectOfType<Room>();
		
		if (room.expired()) {
			Logger::Error("What just happend??");
		}

		auto roomMesh = room.lock();

		roomMesh->transform.SetPosition(DirectX::XMVectorSet(x * 2, 0, y * 2, 0));

		roomMesh->SetParent(this->GetPtr());

		this->rooms[x][y] = room;

		Logger::Log("Created Room");
	}
}

void SpaceShip::Tick() {

	this->GameObject3D::Tick();
	static int pos[2]{};

	ImGui::Begin("Rooms");
	ImGui::InputInt2("cords", pos);
	bool roomCreator = ImGui::Button("Create Room");
	ImGui::End();

	if (roomCreator) {
		this->CreateRoom(pos[0], pos[1]);
	}

}