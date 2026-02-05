#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <vector>
#include <memory>
#include <array>
#include "utilities/logger.h"
#include <DirectXMath.h>
#include "gameObjects/gameObjectFactory.h"

class Scene;
class GameObjectFactory;

class GameObject : public std::enable_shared_from_this<GameObject> {
public:
	GameObject();
	virtual ~GameObject() = default;

	const std::vector<std::weak_ptr<GameObject>>& GetChildren() const;
	const int GetChildCount() const;

	const std::weak_ptr<GameObject> GetParent() const;
	
	/// <summary>
	/// When using this function, the object's current localPosition isn't updated to match it's old worldPosition,
	/// which could lead to unexpected behaviour.
	/// To get desired results, first set parent/children, THEN set position.
	/// </summary>
	/// <param name="newParent"></param>
	virtual void SetParent(std::weak_ptr<GameObject> newParent);

	/// <summary>
	/// Called after the object is instantiated.
	/// Use this rather than the constructor, because the object is actually connected to the Scene here.
	/// </summary>
	virtual void Start();

	virtual void Tick();
	virtual void LateTick();
	virtual void PhysicsTick();
	virtual void LatePhysicsTick();

	// These should be in Transform but that doesn't work because GameObjects doesn't have transforms, only GameObject3D:

	virtual DirectX::XMVECTOR GetGlobalPosition() const;
	virtual DirectX::XMMATRIX GetGlobalWorldMatrix(bool inverseTranspose) const;

	std::weak_ptr<GameObject> GetPtr();

private:
	/// <summary>
	/// WARNING: Engine only. Do not use for any in-game logic.
	/// </summary>
	/// <param name="newChild"></param>
	void AddChild(std::weak_ptr<GameObject> newChild);

	std::vector<std::weak_ptr<GameObject>> children;
	std::weak_ptr<GameObject> parent;
	//std::weak_ptr<GameObject> weakPtr;

	friend Scene;
		
protected:
	GameObjectFactory* factory;
};