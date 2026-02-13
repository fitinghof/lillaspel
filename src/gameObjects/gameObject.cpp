#include "gameObjects/gameObject.h"

GameObject::GameObject() : children(), parent(), factory(nullptr) {
}

const std::vector<std::weak_ptr<GameObject>>& GameObject::GetChildren() const
{
	return this->children;
}

const int GameObject::GetChildCount() const
{
	return this->children.size();
}

const std::weak_ptr<GameObject> GameObject::GetParent() const
{
	return this->parent;
}

void GameObject::SetParent(std::weak_ptr<GameObject> newParent)
{
	if (!this->parent.expired())
	{
		// Remove from the parent's children
		this->parent.lock()->DeleteChild(this->GetPtr());
	}

	if (newParent.expired())
	{
		Logger::Error("Tried to set expired parent.");
		return;
	}

	this->parent = newParent;
	this->parent.lock()->AddChild(this->GetPtr());

	Logger::Log("Set Parent.");
}

void GameObject::AddChild(std::weak_ptr<GameObject> newChild)
{
	if (newChild.expired())
		return;

	this->children.push_back(newChild);
}

void GameObject::DeleteChild(std::weak_ptr<GameObject> oldChild)
{
	GameObject* oldChildPtr = oldChild.lock().get();
	for (size_t i = 0; i < this->children.size(); i++) {
		if (this->children[i].lock().get() == oldChildPtr) {
			this->children.erase(this->children.begin() + i);
		}
		else {
			Logger::Warn("Something went wrong with the delete function.");
		}
	}
}

void GameObject::Start()
{

}

void GameObject::Tick() 
{ 
}

void GameObject::LateTick()
{
}

void GameObject::PhysicsTick()
{
	// TO DO
}

void GameObject::LatePhysicsTick()
{
	// TO DO
}

void GameObject::OnDestroy() {}

DirectX::XMMATRIX GameObject::GetGlobalWorldMatrix(bool inverseTranspose) const
{
	if (this->parent.expired()) {
		return DirectX::XMMatrixIdentity();
	}
	else {
		return this->parent.lock()->GetGlobalWorldMatrix(inverseTranspose);
	}
}

std::shared_ptr<GameObject> GameObject::GetPtr() 
{
	if (this->myPtr.expired()) {
		Logger::Error("How did this happen?");
	}

	return this->myPtr.lock();
	//return shared_from_this();
}

void GameObject::LoadFromJson(const nlohmann::json& data) {
	// Children are loaded by the sceneManager

	if (data.contains("name")) {
		this->name = data.at("name").get<std::string>();
	}
}

void GameObject::SaveToJson(nlohmann::json& data)
{
	data["type"] = "GameObject";
	data["name"] = this->name;

	for (size_t i = 0; i < this->children.size(); i++)
	{
		this->children[i].lock()->SaveToJson(data["children"][i]);
	}
}

void GameObject::ShowInHierarchy() { 
	ImGui::Text("Object details."); 
	if (ImGui::Button("Delete")) {
		this->factory->QueueDeleteGameObject(this->GetPtr());
	}
}

const std::string& GameObject::GetName() { return this->name; }
