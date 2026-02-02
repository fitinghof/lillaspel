#pragma once
#include <string>
#include <array>
#include <vector>
#include "rendering/vertex.h"
#include "rendering/vertexBuffer.h"
#include "rendering/indexBuffer.h"
#include "rendering/texture.h"

class SubMesh {
public:
	SubMesh(size_t startIndex, size_t nrOfIndices, Texture texture);
	SubMesh(SubMesh&&) noexcept = default;
	SubMesh(SubMesh& other) = default;
	~SubMesh();

private:

	size_t startIndex;
	size_t nrOfIndices;
	Texture texture;
};

class Mesh {
public:
	Mesh() = default;
	Mesh(VertexBuffer vertexbuffer, IndexBuffer indexbuffer, std::vector<SubMesh>&& submeshes);
	Mesh(Mesh&&) noexcept = default;
	Mesh& operator=(Mesh&&) noexcept = default;
	~Mesh();

	void Init(VertexBuffer vertexbuffer, IndexBuffer indexbuffer, std::vector<SubMesh>&& submeshes);

	std::string& GetName();
	std::vector<SubMesh>& GetSubMeshes();
	VertexBuffer& GetVertexBuffer();
	IndexBuffer& GetIndexBuffer();

private:
	std::string name;
	std::vector<SubMesh> subMeshes;
	VertexBuffer vertexbuffer;
	IndexBuffer indexbuffer;
};
