#include "gameObjects/gameObjectSerialization.h"

void to_json(nlohmann::json& j, const GameObject3D& obj)
{
	j = nlohmann::json
	{
		{"type", "GameObject3D"},
		{ "transform",
			{"position",
				{ obj.transform.GetPosition().m128_f32[0], obj.transform.GetPosition().m128_f32[2], obj.transform.GetPosition().m128_f32[3]}
			}
		}
	};
}

void from_json(const nlohmann::json& j, GameObject3D& obj)
{
	auto position = j.at("transform").at("position");

	obj.transform.SetPosition(DirectX::XMVectorSet(position[0], position[1], position[2], 1));
}
