#pragma once

#include <memory>

class GameObjectFactory {
public:
    template <typename T>
    std::weak_ptr<T> CreateGameObjectOfType();
    
    // FindObjectOfType

    virtual ~GameObjectFactory() = default;
};