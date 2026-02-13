#include "gameObjects/meshObject.h"

MeshObject::MeshObject() : mesh(), imguiNewMeshIdent("\0") {
	static int id = 0;
	this->tempId = id++;
	Logger::Log("Created a MeshObject.");
}

void MeshObject::SetMesh(MeshObjData newMesh)
{
	this->mesh = newMesh;

	// Should do a check to make sure it isn't already in render queue

	RenderQueue::AddMeshObject(this->GetPtr());
}

MeshObjData MeshObject::GetMesh()
{
	return this->mesh;
}

void MeshObject::Tick()
{
	GameObject3D::Tick();

	//static float rot = 0;
	//this->transform.SetRotationRPY(0,0,rot += 0.0005f);

	// Debug stuff to delete objects
	//ImGui::Begin("Object" + this->tempId);
	//if (ImGui::Button("Add")) {
	//	auto newCam = this->factory->CreateGameObjectOfType<GameObject>();
	//}
	//if (ImGui::Button("Delete")) {
	//	this->factory->QueueDeleteGameObject(this->GetPtr());
	//}
	//ImGui::End();
}

void MeshObject::LoadFromJson(const nlohmann::json& data)
{
	this->GameObject3D::LoadFromJson(data);

	if (data.contains("meshIdentifier")) {
		SetMesh(AssetManager::GetInstance().GetMeshObjData(data["meshIdentifier"].get<std::string>()));
	}
}

void MeshObject::SaveToJson(nlohmann::json& data)
{
	this->GameObject3D::SaveToJson(data);

	data["type"] = "MeshObject";

	data["meshIdentifier"] = GetMesh().GetMeshIdent();
}

void MeshObject::ShowInHierarchy() 
{ 
	this->GameObject3D::ShowInHierarchy();

	ImGui::Text("MeshObject"); 

	if (!this->GetMesh().GetMesh().expired()) {
		std::string meshText = std::format("Mesh: {}", this->GetMesh().GetMeshIdent());
		ImGui::Text(meshText.c_str()); 
	}

	if (ImGui::Button("Change mesh")) ImGui::OpenPopup("change_mesh");
	if (ImGui::BeginPopup("change_mesh")) {
		ImGui::InputText("New Mesh", this->imguiNewMeshIdent, sizeof(this->imguiNewMeshIdent));
		/*if (!std::filesystem::exists(FilepathHolder::GetAssetsDirectory() / this->imguiNewMeshIdent))
			ImGui::Text("Invalid file.");*/
		if (ImGui::Button("Apply")) {
			Logger::Log("Tried to change mesh.");
			this->SetMesh(AssetManager::GetInstance().GetMeshObjData(this->imguiNewMeshIdent));
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}
