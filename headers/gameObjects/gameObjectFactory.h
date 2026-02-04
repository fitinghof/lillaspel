#pragma once

#include <memory>
#include "utilities/logger.h"

class GameObject;

class GameObjectFactory {
public:

    /// <summary>
    /// Creates a new GameObject and registers it in the scene.
    /// T is any type derived from GameObject.
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <returns></returns>
    template <typename T>
    std::weak_ptr<T> CreateGameObjectOfType();
    
    /// <summary>
    /// Delete the GameObject after all game logic is done, but before the game renders.
    /// </summary>
    /// <param name="gameObject"></param>
    virtual void QueueDeleteGameObject(std::weak_ptr<GameObject> gameObject) = 0;
    
    // FindObjectOfType() etc

    virtual ~GameObjectFactory() = default;

private:
    /// <summary>
    /// Add GameObject to game engine logic
    /// </summary>
    /// <param name="gameObject"></param>
    virtual void RegisterGameObject(std::shared_ptr<GameObject> gameObject) = 0;
};

template<typename T>
inline std::weak_ptr<T> GameObjectFactory::CreateGameObjectOfType()
{
    // Make sure it is a gameObject (compiler assert)
    static_assert(std::is_base_of_v<GameObject, T>, "T must derive from GameObject");

    auto obj = std::make_shared<T>();
    RegisterGameObject(obj);
    return obj;
}
