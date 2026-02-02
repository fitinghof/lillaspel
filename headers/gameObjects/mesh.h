#pragma once
#include <string>
#include <array>
#include <vector>
#include "rendering/vertex.h"
#include "rendering/vertexBuffer.h"
#include "rendering/indexBuffer.h"

class SubMesh {
public:
	SubMesh(VertexBuffer vertexbuffer, IndexBuffer indexbuffer);
	SubMesh(SubMesh&&) noexcept = default;
	SubMesh(SubMesh& other) = default;
	~SubMesh();

	VertexBuffer& GetVertexBuffer();
	IndexBuffer& GetIndexBuffer();
private:

	VertexBuffer vertexbuffer;
	IndexBuffer indexbuffer;
	// material index or something?
};

class Mesh {
public:
	Mesh() = default;
	Mesh(std::vector<SubMesh>&& submeshes);
	Mesh(Mesh&&) noexcept = default;
	Mesh& operator=(Mesh&&) noexcept = default;
	~Mesh();

	void Init(std::vector<SubMesh>&& submeshes);

	std::string& GetName();
	std::vector<SubMesh>& GetSubMeshes();

private:
	std::string name;
	std::vector<SubMesh> subMeshes;
};
