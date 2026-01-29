#pragma once

#include <vector>
#include <memory>
#include <array>

class GameObject {
public:
	GameObject();
	virtual ~GameObject() = default;

	const std::vector<GameObject*>& GetChildren() const;
	const int& GetChildCount() const;

	const GameObject* GetParent() const;
	void SetParent(GameObject* newParent);

	/// <summary>
	/// WARNING: Engine only. Do not use for any in-game logic.
	/// </summary>
	/// <param name="newChild"></param>
	void AddChild(GameObject* newChild);

	/// <summary>
	/// Called when the object is instantiated
	/// </summary>
	virtual void Start();

	virtual void Tick();
	virtual void LateTick();
	virtual void PhysicsTick();
	virtual void LatePhysicsTick();

private:
	std::vector<GameObject*> children;
	GameObject* parent;
};