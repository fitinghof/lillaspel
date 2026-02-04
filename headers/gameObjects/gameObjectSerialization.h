#include <nlohmann/json.hpp>
#include "gameObjects/gameObject3D.h"

using json = nlohmann::json;

void nlohmann::to_json(json& j, const GameObject3D& p) {
    j = json{ {"type", "GameObject3D"}, { "transform", }, {"age", p.age}};
}

void from_json(const json& j, person& p) {
    j.at("name").get_to(p.name);
    j.at("address").get_to(p.address);
    j.at("age").get_to(p.age);
}