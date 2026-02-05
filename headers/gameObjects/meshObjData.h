#pragma once
#include <string>
#include "rendering/material.h"
#include "gameObjects/mesh.h"
#include <optional>

#include <memory>

struct SubMeshData {
	SubMesh submesh = SubMesh(0, 0);
	std::weak_ptr<Material> material;
};

class MeshObjData {
public:
	MeshObjData() = default;
	MeshObjData(MeshObjData&&) noexcept = default;
	MeshObjData& operator=(MeshObjData&&) noexcept = default;
	MeshObjData(MeshObjData&)= default;
	MeshObjData& operator=(MeshObjData&) = default;

	void SetMesh(std::shared_ptr<Mesh> mesh);
	void SetMaterial(size_t index, std::shared_ptr<Material> material);
	std::string GetMeshIdent();
	std::weak_ptr<Mesh> GetMesh();
	std::optional<SubMeshData> GetSubMeshData(size_t index);
	std::weak_ptr<Material> GetMaterial(size_t index);
private:
	std::weak_ptr<Mesh> mesh;
	std::vector<std::weak_ptr<Material>> materials;
};
