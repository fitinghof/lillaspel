#include "gameObjects/mesh.h"

Mesh::Mesh(std::vector<SubMesh>&& submeshes) {

}

Mesh::~Mesh() {

}

std::string& Mesh::GetName()
{
    return this->name;
}

std::vector<SubMesh>& Mesh::GetSubMeshes()
{
    return this->subMeshes;
}

SubMesh::SubMesh(VertexBuffer vertexbuffer, IndexBuffer indexbuffer) : vertexbuffer(std::move(vertexbuffer)), indexbuffer(std::move(indexbuffer)) {

}
SubMesh::~SubMesh() {

}

VertexBuffer& SubMesh::GetVertexBuffer()
{
    return this->vertexbuffer;
}

IndexBuffer& SubMesh::GetIndexBuffer()
{
    return this->indexbuffer;
}
