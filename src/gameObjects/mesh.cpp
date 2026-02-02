#include "gameObjects/mesh.h"

Mesh::Mesh(VertexBuffer&& vertexbuffer, IndexBuffer&& indexbuffer, std::vector<SubMesh>&& submeshes) : vertexbuffer(vertexbuffer), indexbuffer(indexbuffer) {
    // submeshes
}

Mesh::~Mesh() {

}

VertexBuffer& Mesh::GetVertexBuffer()
{
    return this->vertexbuffer;
}

IndexBuffer& Mesh::GetIndexBuffer()
{
    return this->indexbuffer;
}

std::string& Mesh::GetName()
{
    return this->name;
}

std::vector<SubMesh>& Mesh::GetSubMeshes()
{
    return this->subMeshes;
}

SubMesh::SubMesh(size_t startIndex, size_t nrOfIndicies) {

}
SubMesh::~SubMesh() {

}
