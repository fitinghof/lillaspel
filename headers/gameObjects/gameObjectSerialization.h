#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include "utilities/logger.h"
#include <iomanip>
#include <nlohmann/json.hpp>
#include "gameObjects/gameObject3D.h"

//using json = nlohmann::json;

void to_json(nlohmann::json& j, const GameObject3D& obj);

void from_json(const nlohmann::json& j, GameObject3D& obj);