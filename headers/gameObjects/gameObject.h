#pragma once

#include <vector>
#include <memory>
#include <array>
#include "utilities/logger.h"
#include <DirectXMath.h>

class Scene;

class GameObject {
public:
	GameObject();
	virtual ~GameObject() = default;

	const std::vector<std::weak_ptr<GameObject>>& GetChildren() const;
	const int GetChildCount() const;

	const std::weak_ptr<GameObject> GetParent() const;
	void SetParent(std::weak_ptr<GameObject> newParent);

	/// <summary>
	/// WARNING: Engine only. Do not use for any in-game logic.
	/// </summary>
	/// <param name="newChild"></param>
	void AddChild(std::weak_ptr<GameObject> newChild);

	/// <summary>
	/// Called when the object is instantiated
	/// </summary>
	virtual void Start();

	virtual void Tick();
	virtual void LateTick();
	virtual void PhysicsTick();
	virtual void LatePhysicsTick();

	virtual DirectX::XMVECTOR GetGlobalPosition() const;
	//virtual DirectX::XMVECTOR GetGlobalRotation() const;
	//virtual DirectX::XMVECTOR GetGlobalScale() const;
	//virtual DirectX::XMFLOAT4X4 GetGlobalWorldMatrix(bool inverseTranspose) const;

private:
	std::vector<std::weak_ptr<GameObject>> children;
	std::weak_ptr<GameObject> parent;
	std::weak_ptr<GameObject> weakPtr;
	
	void SetWeakPtr(std::weak_ptr<GameObject> yourPtr);
	
	friend Scene;
};