#pragma once
#include <string>
#include <array>
#include <vector>
#include "rendering/vertex.h"
#include "rendering/vertexBuffer.h"
#include "rendering/indexBuffer.h"

class SubMesh {
public:
	SubMesh(size_t startIndex, size_t nrOfIndicies);
	SubMesh(SubMesh&&) noexcept = default;
	SubMesh(SubMesh& other) = default;
	~SubMesh();


private:
	size_t startIndex;
	size_t nrOfIndicies;
	//Materials?
};

class Mesh {
public:
	Mesh(VertexBuffer&& vertexbuffer, IndexBuffer&& indexbuffer, std::vector<SubMesh>&& submeshes);
	Mesh(Mesh&&) noexcept = default;
	~Mesh();

private:
	std::string name;
	VertexBuffer vertexbuffer;
	IndexBuffer indexbuffer;
	std::vector<SubMesh> subMeshes;
};
